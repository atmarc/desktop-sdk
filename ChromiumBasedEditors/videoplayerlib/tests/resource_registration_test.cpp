// SPDX-License-Identifier: AGPL-3.0-only
#include "../qascvideoview.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	NSBaseVideoLibrary::Init(nullptr);
	if (!QFile::exists(":/icons/btn-play-light.png") || !QFile::exists(":/icons/btn-pause-dark.svg"))
	{
		qCritical() << "Media control icon resources are missing";
		return 1;
	}
	qInfo() << "PASS: media control icons are linked and registered";
	return 0;
}
