#ifndef LH_QT_CONTAINER_H
#define LH_QT_CONTAINER_H

#include <litehtml.h>
#include <QUrl>
#include <QPixmap>
#include <QHash>
#include <QCursor>

class IContainerCallbacks;


class qt_container : public litehtml::document_container
{
public:
    qt_container(IContainerCallbacks& callbacks):callbacks_{callbacks} {}

    virtual litehtml::uint_ptr		create_font(const char* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) override;
    virtual void			delete_font(litehtml::uint_ptr hFont) override;
    virtual int				text_width(const char* text, litehtml::uint_ptr hFont) override;
    virtual void			draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) override;
    virtual int				pt_to_px(int pt) const override;
    virtual int				get_default_font_size() const override;
    virtual const char*		get_default_font_name() const override;
    virtual void 			load_image(const char* src, const char* baseurl, bool redraw_on_ready) override;
    virtual void			get_image_size(const char* src, const char* baseurl, litehtml::size& sz) override;
    virtual void			draw_background(litehtml::uint_ptr hdc, const std::vector<litehtml::background_paint>& bg) override;
    virtual void			draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) override;
    virtual void 			draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) override;
    virtual litehtml::element::ptr	create_element(const char* tag_name,
                                        const litehtml::string_map& attributes,
                                        const litehtml::document::ptr& doc) override;
    virtual void			get_media_features(litehtml::media_features& media) const override;
    virtual void			get_language(litehtml::string& language, litehtml::string& culture) const override;
    virtual void 			link(const litehtml::document::ptr& doc, const litehtml::element::ptr& el) override;

    virtual void			transform_text(litehtml::string& text, litehtml::text_transform tt) override;
    virtual void			set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius) override;
    virtual void			del_clip() override ;

    virtual void 			set_caption(const char* caption) override;
    virtual void 			set_base_url(const char* base_url) override;
    virtual void			on_anchor_click(const char* url, const litehtml::element::ptr& el) override;
    virtual void			set_cursor(const char* cursor) override;
    virtual void			import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl) override;
    virtual void			get_client_rect(litehtml::position& client) const override;

private:
    QUrl resolveUrl(const QString &url, const QString &baseUrl) const;
    QPixmap getPixmap(const QString &imageUrl, const QString &baseUrl);

    QString baseUrl_;
    float ptTpPxConversion_{-1.0};
    QHash<QUrl, QPixmap> pixmaps_;
    QCursor currentCursor_;
    IContainerCallbacks &callbacks_;

};

#endif // LH_QT_CONTAINER_H