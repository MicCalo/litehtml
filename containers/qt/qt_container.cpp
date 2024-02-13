#include "qt_container.h"
#include "qt_conversion.h"
#include "libs/litehtml/include/litehtml/background.h"

#include <QFile>
#include <QDir>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QDebug>


using namespace litehtml;


QPixmap qt_container::getPixmap(const QString &imageUrl, const QString &baseUrl)
{
    const QUrl url = resolveUrl(imageUrl, baseUrl);
    if (!pixmaps_.contains(url)) {
        qWarning() << "draw_background: pixmap not loaded for" << url;
        return {};
    }
    return pixmaps_.value(url);
}


// note: font is selected only by size, name and style are not used
uint_ptr qt_container::create_font(const char* faceName, int size, int weight, font_style italic, unsigned int decoration, font_metrics* fm)
{
    const QStringList splitNames = QString::fromUtf8(faceName).split(',', Qt::SkipEmptyParts);
    QStringList familyNames;
    std::transform(splitNames.cbegin(),
                   splitNames.cend(),
                   std::back_inserter(familyNames),
                   [this](const QString &s) {
                       // clean whitespace and quotes
                       QString name = s.trimmed();
                       if (name.startsWith('\"'))
                           name = name.mid(1);
                       if (name.endsWith('\"'))
                           name.chop(1);
                       const QString lowerName = name.toLower();
                       if (lowerName == "serif")
                           return QString("Times New Roman");
                       if (lowerName == "sans-serif")
                           return QString("Arial");
                       if (lowerName == "monospace")
                           return QString("Courier");
                       return name;
                   });

    auto font = new QFont();
    font->setFamilies(familyNames);
    font->setPixelSize(size);
    font->setWeight(static_cast<QFont::Weight>(weight));
    font->setStyle(qt_conversion::toQFontStyle(italic));

    bool drawSpaces{false};
    if (decoration == litehtml::font_decoration_underline){
        font->setUnderline(true);
        drawSpaces = true;
    }
    if (decoration == litehtml::font_decoration_overline){
        font->setOverline(true);
        drawSpaces = true;
    }
    if (decoration == litehtml::font_decoration_linethrough){
        font->setStrikeOut(true);
        drawSpaces = true;
    }
    if (fm) {
        const QFontMetrics metrics(*font);
        fm->height = metrics.height();
        fm->ascent = metrics.ascent();
        fm->descent = metrics.descent();
        fm->x_height = metrics.xHeight();
        fm->draw_spaces = drawSpaces;
    }
    return reinterpret_cast<litehtml::uint_ptr>(font);
}

void qt_container::delete_font(uint_ptr hFont){
    QFont* pFont{reinterpret_cast<QFont*>(hFont)};
    delete pFont;
}

int qt_container::text_width(const char* text, uint_ptr hFont)
{
    QFont* pFont{reinterpret_cast<QFont*>(hFont)};
    QFontMetrics fm{*pFont};

    int result{ fm.horizontalAdvance(text)};
    return result;
}

void qt_container::draw_text(uint_ptr hdc, const char* text, uint_ptr hFont, web_color color, const position& position)
{
    QPainter* pPainter{reinterpret_cast<QPainter*>(hdc)};

    QFont* pFont{reinterpret_cast<QFont*>(hFont)};

    pPainter->setFont(*pFont);
    pPainter->setPen(qt_conversion::toQColor(color));
    pPainter->drawText(qt_conversion::toQRect(position), 0, text);

    if (ptTpPxConversion_<0.0){
        ptTpPxConversion_ = pPainter->device()->physicalDpiY() / pPainter->device()->logicalDpiY();
    }
}

int qt_container::pt_to_px(int pt) const {
    if (ptTpPxConversion_>0){
        return pt * ptTpPxConversion_;
    }
    return pt * 96 / 72;
    // magic factor of 11/12 to account for differences to webengine/webkit
    //return m_paintDevice->physicalDpiY() * pt * 11 / m_paintDevice->logicalDpiY() / 12;
  //  return pt * 96 / 72;
}

int qt_container::get_default_font_size() const {
    return 16;
}

const char* qt_container::get_default_font_name() const {
    return "Segeo UI";
}

void qt_container::load_image(const char* src, const char* baseurl, bool redraw_on_ready) {
    //qDebug()<<"load_image"<<src<<baseurl;
    const auto qtSrc = QString::fromUtf8(src);
    const auto qtBaseUrl = QString::fromUtf8(baseurl);
    Q_UNUSED(redraw_on_ready)
    qDebug() << "load_image:" << QString("src = \"%1\";").arg(qtSrc).toUtf8().constData()
                << QString("base = \"%1\"").arg(qtBaseUrl).toUtf8().constData();
    const QUrl url = resolveUrl(qtSrc, qtBaseUrl);
    if (pixmaps_.contains(url))
        return;

    QPixmap pixmap;
    pixmap.loadFromData(callbacks_.loadData(url));
    pixmaps_.insert(url, pixmap);
}

void qt_container::get_image_size(const char* src, const char* baseurl, size& sz) {
    //qDebug()<<"get_image_size";
    const auto qtSrc = QString::fromUtf8(src);
    const auto qtBaseUrl = QString::fromUtf8(baseurl);
    if (qtSrc.isEmpty()) // for some reason that happens
        return;
    /*
    qDebug() << "get_image_size:" << QString("src = \"%1\";").arg(qtSrc).toUtf8().constData()
                << QString("base = \"%1\"").arg(qtBaseUrl).toUtf8().constData();*/
    const QPixmap pm = getPixmap(qtSrc, qtBaseUrl);
    sz.width = pm.width();
    sz.height = pm.height();
}

void qt_container::draw_background(uint_ptr hdc, const std::vector<background_paint>& backgrounds)
{
    QPainter* painter{reinterpret_cast<QPainter*>(hdc)};
    painter->save();

    const auto& bg = backgrounds.back();

    //rounded_rectangle(cr, bg.border_box, bg.border_radius);
    //painter->setClipRect(qt_conversion::toQRect(bg.border_box));

    //cairo_rectangle(cr, bg.clip_box.x, bg.clip_box.y, bg.clip_box.width, bg.clip_box.height);
    //painter->setClipRect(qt_conversion::toQRect(bg.clip_box));

    if(bg.color.alpha)
    {
        //set_color(cr, bg.color);
        //cairo_paint(cr);
    }

    for (int i = (int)backgrounds.size() - 1; i >= 0; i--)
    {
    //for(const auto& bg: bgs){
        const background_paint& bg=backgrounds[i];

        if (bg.is_root){
            //continue;
        }
        painter->setClipRect(qt_conversion::toQRect(bg.clip_box));
        const QRegion horizontalMiddle(
            QRect(bg.border_box.x,
                  bg.border_box.y + bg.border_radius.top_left_y,
                  bg.border_box.width,
                  bg.border_box.height - bg.border_radius.top_left_y - bg.border_radius.bottom_left_y));
        const QRegion horizontalTop(
            QRect(bg.border_box.x + bg.border_radius.top_left_x,
                  bg.border_box.y,
                  bg.border_box.width - bg.border_radius.top_left_x - bg.border_radius.top_right_x,
                  bg.border_radius.top_left_y));
        const QRegion horizontalBottom(QRect(bg.border_box.x + bg.border_radius.bottom_left_x,
                                             bg.border_box.bottom() - bg.border_radius.bottom_left_y,
                                             bg.border_box.width - bg.border_radius.bottom_left_x
                                                 - bg.border_radius.bottom_right_x,
                                             bg.border_radius.bottom_left_y));
        const QRegion topLeft(QRect(bg.border_box.left(),
                                    bg.border_box.top(),
                                    2 * bg.border_radius.top_left_x,
                                    2 * bg.border_radius.top_left_y),
                              QRegion::Ellipse);
        const QRegion topRight(QRect(bg.border_box.right() - 2 * bg.border_radius.top_right_x,
                                     bg.border_box.top(),
                                     2 * bg.border_radius.top_right_x,
                                     2 * bg.border_radius.top_right_y),
                               QRegion::Ellipse);
        const QRegion bottomLeft(QRect(bg.border_box.left(),
                                       bg.border_box.bottom() - 2 * bg.border_radius.bottom_left_y,
                                       2 * bg.border_radius.bottom_left_x,
                                       2 * bg.border_radius.bottom_left_y),
                                 QRegion::Ellipse);
        const QRegion bottomRight(QRect(bg.border_box.right() - 2 * bg.border_radius.bottom_right_x,
                                        bg.border_box.bottom() - 2 * bg.border_radius.bottom_right_y,
                                        2 * bg.border_radius.bottom_right_x,
                                        2 * bg.border_radius.bottom_right_y),
                                  QRegion::Ellipse);
        const QRegion clipRegion = horizontalMiddle.united(horizontalTop)
                                       .united(horizontalBottom)
                                       .united(topLeft)
                                       .united(topRight)
                                       .united(bottomLeft)
                                       .united(bottomRight);
        painter->setClipRegion(clipRegion, Qt::IntersectClip);
        painter->setPen(Qt::NoPen);
        painter->setBrush(qt_conversion::toQColor(bg.color));
        painter->drawRect(bg.border_box.x, bg.border_box.y, bg.border_box.width, bg.border_box.height);
        //drawSelection(painter, qt_conversion::toQRect(bg.border_box));
        if (!bg.image.empty()) {
            const QPixmap pixmap = getPixmap(QString::fromStdString(bg.image),
                                             QString::fromStdString(bg.baseurl));
            if (bg.repeat == litehtml::background_repeat_no_repeat) {
                painter->drawPixmap(QRect(bg.position_x,
                                          bg.position_y,
                                          bg.image_size.width,
                                          bg.image_size.height),
                                    pixmap);
            } else if (bg.repeat == litehtml::background_repeat_repeat_x) {
                if (bg.image_size.width > 0) {
                    int x = bg.border_box.left();
                    while (x <= bg.border_box.right()) {
                        painter->drawPixmap(QRect(x,
                                                  bg.border_box.top(),
                                                  bg.image_size.width,
                                                  bg.image_size.height),
                                            pixmap);
                        x += bg.image_size.width;
                    }
                }
            } else {
                qWarning() << "unsupported background repeat" << bg.repeat;
            }
        }
    }
    painter->restore();
}

void qt_container::draw_borders(uint_ptr hdc, const borders& borders, const position& draw_pos, bool root)
{
    Q_UNUSED(root)
    // TODO: special border styles
    QPainter* painter{reinterpret_cast<QPainter*>(hdc)};

    if (borders.top.style != litehtml::border_style_none
        && borders.top.style != litehtml::border_style_hidden) {
        painter->setPen(qt_conversion::getQPen(borders.top));
        painter->drawLine(draw_pos.left() + borders.radius.top_left_x,
                          draw_pos.top(),
                          draw_pos.right() - borders.radius.top_right_x,
                          draw_pos.top());
        painter->drawArc(draw_pos.left(),
                         draw_pos.top(),
                         2 * borders.radius.top_left_x,
                         2 * borders.radius.top_left_y,
                         90 * 16,
                         90 * 16);
        painter->drawArc(draw_pos.right() - 2 * borders.radius.top_right_x,
                         draw_pos.top(),
                         2 * borders.radius.top_right_x,
                         2 * borders.radius.top_right_y,
                         0,
                         90 * 16);
    }
    if (borders.bottom.style != litehtml::border_style_none
        && borders.bottom.style != litehtml::border_style_hidden) {
        painter->setPen(qt_conversion::getQPen(borders.bottom));
        painter->drawLine(draw_pos.left() + borders.radius.bottom_left_x,
                          draw_pos.bottom(),
                          draw_pos.right() - borders.radius.bottom_right_x,
                          draw_pos.bottom());
        painter->drawArc(draw_pos.left(),
                         draw_pos.bottom() - 2 * borders.radius.bottom_left_y,
                         2 * borders.radius.bottom_left_x,
                         2 * borders.radius.bottom_left_y,
                         180 * 16,
                         90 * 16);
        painter->drawArc(draw_pos.right() - 2 * borders.radius.bottom_right_x,
                         draw_pos.bottom() - 2 * borders.radius.bottom_right_y,
                         2 * borders.radius.bottom_right_x,
                         2 * borders.radius.bottom_right_y,
                         270 * 16,
                         90 * 16);
    }
    if (borders.left.style != litehtml::border_style_none
        && borders.left.style != litehtml::border_style_hidden) {
        painter->setPen(qt_conversion::getQPen(borders.left));
        painter->drawLine(draw_pos.left(),
                          draw_pos.top() + borders.radius.top_left_y,
                          draw_pos.left(),
                          draw_pos.bottom() - borders.radius.bottom_left_y);
    }
    if (borders.right.style != litehtml::border_style_none
        && borders.right.style != litehtml::border_style_hidden) {
        painter->setPen(qt_conversion::getQPen(borders.right));
        painter->drawLine(draw_pos.right(),
                          draw_pos.top() + borders.radius.top_right_y,
                          draw_pos.right(),
                          draw_pos.bottom() - borders.radius.bottom_right_y);
    }
}

void qt_container::draw_list_marker(uint_ptr hdc, const list_marker& marker)
{
    //qDebug()<<"draw_list_marker";
    QPainter* painter{reinterpret_cast<QPainter*>(hdc)};

    if (marker.image.empty()) {
        if (marker.marker_type == litehtml::list_style_type_square) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(qt_conversion::toQColor(marker.color));
            painter->drawRect(qt_conversion::toQRect(marker.pos));
        } else if (marker.marker_type == litehtml::list_style_type_disc) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(qt_conversion::toQColor(marker.color));
            painter->drawEllipse(qt_conversion::toQRect(marker.pos));
        } else if (marker.marker_type == litehtml::list_style_type_circle) {
            painter->setPen(qt_conversion::toQColor(marker.color));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(qt_conversion::toQRect(marker.pos));
        } else {
            // TODO we do not get information about index and font for e.g. decimal / roman
            // at least draw a bullet
            painter->setPen(Qt::NoPen);
            painter->setBrush(qt_conversion::toQColor(marker.color));
            painter->drawEllipse(qt_conversion::toQRect(marker.pos));
            qWarning() << "list marker of type" << marker.marker_type << "not supported";
        }
    } else {
        const QPixmap pixmap = getPixmap(QString::fromStdString(marker.image),
                                         QString::fromStdString(marker.baseurl));
        painter->drawPixmap(qt_conversion::toQRect(marker.pos), pixmap);
    }
}

element::ptr qt_container::create_element(const char* tag_name,
                           const string_map& attributes,
                         const document::ptr& doc) {
    return nullptr;
}

void qt_container::get_media_features(media_features& media) const {
    /*
    position size;
    get_client_rect(size);

    media.type = litehtml::media_type_screen;
    media.height=size.height;
    media.width = size.width;
    media.resolution = 96;
*/
}

void qt_container::get_language(string& language, string& culture) const {
    language = "en";
    culture = "";
}

void qt_container::link(const document::ptr& doc, const element::ptr& el) {
    qDebug()<<"link";
}

void qt_container::transform_text(string& text, text_transform tt) {
    switch (tt){
        case litehtml::text_transform_none:
            break;
        case litehtml::text_transform_capitalize:
            //text = QString::fromUtf8(text).toCaseFolded().toStdString();
            break;
        case litehtml::text_transform_uppercase:
            text = QString::fromUtf8(text).toUpper().toStdString();
            break;
        case litehtml::text_transform_lowercase:
            text = QString::fromUtf8(text).toLower().toStdString();
            break;
        }

}

void qt_container::set_clip(const position& pos, const border_radiuses& bdr_radius) {
    qDebug()<<"set_clip";
}

void qt_container::del_clip() {
    qDebug()<<"del_clip";
}

void qt_container::set_caption(const char* caption) {
    //qDebug()<<"set_caption"<<caption;
    callbacks_.setCaption(caption);
}

void qt_container::set_base_url(const char* base_url) {
    //qDebug()<<"set_base_url:"<<base_url;
    baseUrl_ = QString::fromUtf8(base_url);
}

void qt_container::on_anchor_click(const char* url, const element::ptr& el) {
    //qDebug()<<"on_anchor_click";
    callbacks_.openLink(resolveUrl(QString::fromUtf8(url), baseUrl_));
}

void qt_container::set_cursor(const char* cursor) {
    QCursor qtCursor{qt_conversion::toQCursor(cursor)};
    if (currentCursor_ != qtCursor){
        //qDebug()<<"set_cursor"<<cursor;
        currentCursor_ = qtCursor;
        callbacks_.setCursor(currentCursor_);
    }
}

QUrl qt_container::resolveUrl(const QString &url, const QString &baseUrl) const
{
    // several cases:
    // full url: "https://foo.bar/blah.css"
    // relative path: "foo/bar.css"
    // server relative path: "/foo/bar.css"
    // net path: "//foo.bar/blah.css"
    // fragment only: "#foo-fragment"
    const QUrl qurl = QUrl::fromEncoded(url.toUtf8());
    if (qurl.scheme().isEmpty()) {
        if (url.startsWith('#')) // leave alone if just a fragment
            return qurl;
        const QUrl pageBaseUrl = QUrl(baseUrl.isEmpty() ? baseUrl_ : baseUrl);
        if (url.startsWith("//")) // net path
            return QUrl(pageBaseUrl.scheme() + ":" + url);
        QUrl serverUrl = QUrl(pageBaseUrl);
        serverUrl.setPath("");
        const QString actualBaseUrl = url.startsWith('/')
                                          ? serverUrl.toString(QUrl::FullyEncoded)
                                          : pageBaseUrl.toString(QUrl::FullyEncoded);
        QUrl resolvedUrl(actualBaseUrl + '/' + url);
        resolvedUrl.setPath(resolvedUrl.path(QUrl::FullyEncoded | QUrl::NormalizePathSegments), QUrl::TolerantMode);
        return resolvedUrl;
    }
    return qurl;
}

void qt_container::import_css(string& text, const string& url, string& baseurl)
{
    const QUrl actualUrl = resolveUrl(QString::fromStdString(url), QString::fromStdString(baseurl));
    const QString urlString = actualUrl.toString(QUrl::None);
    const int lastSlash = urlString.lastIndexOf('/');

    baseurl = urlString.left(lastSlash).toStdString();
    text = QString::fromUtf8(callbacks_.loadData(actualUrl)).toStdString();
}

void qt_container::get_client_rect(position& client) const
{
    client = position{0,0,555, 777};
}
