#include "qt_conversion.h"
#include <QDebug>
#include <QColor>
#include <QRect>
#include <QCursor>
#include <QPen>

QFont::Style qt_conversion::toQFontStyle(litehtml::font_style style)
{
    switch (style) {
    case litehtml::font_style_normal:
        return QFont::StyleNormal;
    case litehtml::font_style_italic:
        return QFont::StyleItalic;
    }
    // should not happen
    qWarning() << "Unknown litehtml font style:" << style;
    return QFont::StyleNormal;
}

QColor qt_conversion::toQColor(const litehtml::web_color &color)
{
    return QColor{color.red, color.green, color.blue, color.alpha};
}

QRect qt_conversion::toQRect(const litehtml::position &position)
{
    return QRect{position.x, position.y, position.width, position.height};
}


Qt::PenStyle qt_conversion::toQPenStyle(const litehtml::border_style style)
{
    switch (style) {
    case litehtml::border_style_dotted:
        return Qt::DotLine;
    case litehtml::border_style_dashed:
        return Qt::DashLine;
    case litehtml::border_style_solid:
        return Qt::SolidLine;
    /*
    case litehtml::border_style_double:
        return Qt::SolidLine;
    case litehtml::border_style_groove:
        return Qt::SolidLine;
    case litehtml::border_style_ridge:
        return Qt::SolidLine;
    case litehtml::border_style_inset:
        return Qt::SolidLine;
    case litehtml::border_style_outset:
        return Qt::SolidLine;
*/
    default:
       // qWarning() << "Unsupported border style:" << style;
        break;
    }

    return Qt::SolidLine;
}

QPen qt_conversion::getQPen(const litehtml::border &border)
{
    return QPen{toQColor(border.color), qreal(border.width), toQPenStyle(border.style)};
}

QCursor qt_conversion::toQCursor(const QString &c)
{
    if (c == "alias")
        return {Qt::PointingHandCursor}; // ???
    if (c == "all-scroll")
        return {Qt::SizeAllCursor};
    if (c == "auto")
        return {Qt::ArrowCursor}; // ???
    if (c == "cell")
        return {Qt::UpArrowCursor};
    if (c == "context-menu")
        return {Qt::ArrowCursor}; // ???
    if (c == "col-resize")
        return {Qt::SplitHCursor};
    if (c == "copy")
        return {Qt::DragCopyCursor};
    if (c == "crosshair")
        return {Qt::CrossCursor};
    if (c == "default")
        return {Qt::ArrowCursor};
    if (c == "e-resize")
        return {Qt::SizeHorCursor}; // ???
    if (c == "ew-resize")
        return {Qt::SizeHorCursor};
    if (c == "grab")
        return {Qt::OpenHandCursor};
    if (c == "grabbing")
        return {Qt::ClosedHandCursor};
    if (c == "help")
        return {Qt::WhatsThisCursor};
    if (c == "move")
        return {Qt::SizeAllCursor};
    if (c == "n-resize")
        return {Qt::SizeVerCursor}; // ???
    if (c == "ne-resize")
        return {Qt::SizeBDiagCursor}; // ???
    if (c == "nesw-resize")
        return {Qt::SizeBDiagCursor};
    if (c == "ns-resize")
        return {Qt::SizeVerCursor};
    if (c == "nw-resize")
        return {Qt::SizeFDiagCursor}; // ???
    if (c == "nwse-resize")
        return {Qt::SizeFDiagCursor};
    if (c == "no-drop")
        return {Qt::ForbiddenCursor};
    if ((c == "none") || (c == ""))
        return {Qt::BlankCursor};
    if (c == "not-allowed")
        return {Qt::ForbiddenCursor};
    if (c == "pointer")
        return {Qt::PointingHandCursor};
    if (c == "progress")
        return {Qt::BusyCursor};
    if (c == "row-resize")
        return {Qt::SplitVCursor};
    if (c == "s-resize")
        return {Qt::SizeVerCursor}; // ???
    if (c == "se-resize")
        return {Qt::SizeFDiagCursor}; // ???
    if (c == "sw-resize")
        return {Qt::SizeBDiagCursor}; // ???
    if (c == "text")
        return {Qt::IBeamCursor};
    if (c == "url")
        return {Qt::ArrowCursor}; // ???
    if (c == "w-resize")
        return {Qt::SizeHorCursor}; // ???
    if (c == "wait")
        return {Qt::BusyCursor};
    if (c == "zoom-in")
        return {Qt::ArrowCursor}; // ???
    qWarning() << QString("unknown cursor property \"%1\"").arg(c).toUtf8().constData();
    return {Qt::ArrowCursor};
}
