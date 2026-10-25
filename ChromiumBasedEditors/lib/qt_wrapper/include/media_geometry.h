// SPDX-License-Identifier: AGPL-3.0-only
#pragma once

#include <QPoint>
#include <QRect>
#include <QtMath>

namespace MediaGeometry
{

// CEF media rectangles use physical pixels; QWidget geometry uses logical
// pixels. Parent offsets have already been measured in QWidget coordinates.
inline QRect fromNativePixels(const QRect& rect, qreal pixelRatio, const QPoint& offset = QPoint())
{
	return QRect(offset.x() + qRound(rect.x() / pixelRatio),
				 offset.y() + qRound(rect.y() / pixelRatio),
				 qRound(rect.width() / pixelRatio),
				 qRound(rect.height() / pixelRatio));
}

}
