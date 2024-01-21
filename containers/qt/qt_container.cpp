#include "qt_container.h"

string readfile(string filename);

// note: font is selected only by size, name and style are not used
uint_ptr qt_container::create_font(const char* faceName, int size, int weight, font_style italic, unsigned int decoration, font_metrics* fm)
{	
    return 0;
}

int qt_container::text_width(const char* text, uint_ptr hFont)
{
    return 0;
}

void qt_container::draw_text(uint_ptr hdc, const char* text, uint_ptr hFont, web_color color, const position& pos)
{

}

int qt_container::pt_to_px(int pt) const { return pt * 96 / 72; }
int qt_container::get_default_font_size() const { return 16; }
const char* qt_container::get_default_font_name() const { return ""; }

void qt_container::draw_background(uint_ptr hdc, const std::vector<background_paint>& bg)
{

}

void qt_container::draw_borders(uint_ptr hdc, const borders& borders, const position& pos, bool root)
{

}

void qt_container::draw_list_marker(uint_ptr hdc, const list_marker& marker)
{

}

void qt_container::import_css(string& text, const string& url, string& baseurl)
{

}

void qt_container::get_client_rect(position& client) const
{

}
