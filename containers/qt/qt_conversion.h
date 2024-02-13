#ifndef LH_QT_CONVERSIONS_H
#define LH_QT_CONVERSIONS_H

#include "litehtml.h"

#include <QFont>
#include <QUrl>

class IContainerCallbacks{
    public:
        virtual void openLink(const QUrl & url) = 0;
        virtual void setCaption(const QString &caption) = 0;
        virtual void setCursor(const QCursor& cursor) = 0;
        virtual QByteArray loadData(const QUrl& url) const = 0;
};

class qt_conversion
{
    public:
        static QFont::Style toQFontStyle(const litehtml::font_style style);
        static QColor toQColor(const litehtml::web_color &color);
        static QRect toQRect(const litehtml::position &position);
        static QPen getQPen(const litehtml::border &border);
        static QCursor toQCursor(const QString &c);

    private:
        static Qt::PenStyle toQPenStyle(const litehtml::border_style style);
};

#endif // LH_QT_CONVERSIONS_H
