// SPDX-License-Identifier: AGPL-3.0-only
// Exercise the real playlist probe without starting the desktop editor or CEF.
#include "../src/qvideoplaylist.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSet>
#include <QTemporaryDir>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	if (argc != 2 || !QFileInfo::exists(QString::fromLocal8Bit(argv[1])))
	{
		qCritical() << "Usage: videoplayer_probe_test /absolute/path/to/video.mp4";
		return 2;
	}

	QTemporaryDir directory;
	if (!directory.isValid())
		return 2;
	const QString source = QFileInfo(QString::fromLocal8Bit(argv[1])).absoluteFilePath();
	QStringList files;
	// Distinct sources exercise successive probes and source-reset signals.
	for (int i = 0; i < 10; ++i)
	{
		const QString path = directory.filePath(QString("video-%1.mp4").arg(i));
		if (!QFile::link(source, path))
			return 2;
		files.append(path);
	}
	// Error/InvalidMedia notifications must complete once and not stall the queue.
	files.prepend(directory.filePath("missing.mp4"));
	const QSet<QString> expected(files.begin(), files.end());
	QSet<QString> completed;
	QVideoPlaylist playlist;
	auto *model = playlist.m_pListView->model();
	QObject::connect(model, &QAbstractItemModel::dataChanged, &app,
		[&](const QModelIndex &first, const QModelIndex &last) {
			for (int row = first.row(); row <= last.row(); ++row)
			{
				const QString duration = model->index(row, 1).data().toString();
				if (duration == "...")
					continue;
				const QString path = model->index(row, 0).data(Qt::AccessibleTextRole).toString();
				if (!expected.contains(path) || completed.contains(path))
				{
					qCritical() << "Unexpected/duplicate probe result:" << path;
					app.exit(1);
					return;
				}
				if (path.endsWith("missing.mp4") ? duration != "00:00" : duration == "00:00")
				{
					qCritical() << "Unexpected duration:" << path << duration;
					app.exit(1);
					return;
				}
				completed.insert(path);
				qInfo() << "Probe completed:" << QFileInfo(path).fileName() << duration;
			}
			if (completed == expected)
				QTimer::singleShot(100, &app, [&]() {
					qInfo() << "PASS: ten valid probes and one invalid source";
					app.exit(0);
				});
		});
	QTimer::singleShot(20000, &app, [&]() {
		qCritical() << "Timed out:" << completed.size() << "of" << expected.size();
		app.exit(1);
	});
	playlist.AddFiles(files);
	return app.exec();
}
