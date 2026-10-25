// SPDX-License-Identifier: AGPL-3.0-only
#include "../../lib/qt_wrapper/include/media_geometry.h"
#include <QDebug>

int main()
{
	struct Case
	{
		QRect native;
		qreal pixelRatio;
		QPoint offset;
		QRect expected;
	};
	const Case cases[] = {
		{QRect(125, 250, 375, 500), 1.0, QPoint(), QRect(125, 250, 375, 500)},
		{QRect(125, 250, 375, 500), 1.25, QPoint(), QRect(100, 200, 300, 400)},
		{QRect(125, 250, 375, 500), 1.25, QPoint(0, 28), QRect(100, 228, 300, 400)},
		{QRect(150, 300, 450, 600), 1.5, QPoint(10, 20), QRect(110, 220, 300, 400)},
		{QRect(200, 400, 600, 800), 2.0, QPoint(), QRect(100, 200, 300, 400)},
		// Actual slideshow rectangle from the 125%-scale reproduction.
		{QRect(371, 351, 275, 276), 1.25, QPoint(), QRect(297, 281, 220, 221)},
	};
	for (const Case& test : cases)
	{
		const QRect actual = MediaGeometry::fromNativePixels(test.native, test.pixelRatio, test.offset);
		if (actual != test.expected)
		{
			qCritical() << "Geometry mismatch:" << actual << "expected" << test.expected;
			return 1;
		}
	}
	qInfo() << "PASS: media geometry at 100%, 125%, 150%, 200%, logical parent offsets and slideshow rounding";
	return 0;
}
