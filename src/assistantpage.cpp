/**
 * @file
 * AssistantPage class implementation.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#include "assistantpage.hpp"
#include <gtkmm/cssprovider.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Constructor.
 * @param header Assistant page header text.
 */
AssistantPage::AssistantPage(const Glib::ustring &header)
{
    auto css_provider = Gtk::CssProvider::create();
    Glib::ustring css_data = "GtkLabel.H1 {"
                             "    padding: 10px;"
                             "    font-size: 2em;"
                             "    font-weight: bold;"
                             "    border-radius: 5px 0 0 5px;"
                             "    color: gold;"
                             "    background-image: -gtk-gradient(linear, left top, right top, from(olivedrab), color-stop(0.5, olive), to(alpha(olive, 0)));"
                             "}";

    this->m_header_label = Gtk::manage(new Gtk::Label(header));
    this->m_content_area = Gtk::manage(new Gtk::Grid);

    this->m_header_label->set_hexpand();
    this->m_header_label->get_style_context()->add_class("H1");
    this->m_header_label->set_margin_bottom(30);

    this->m_header_label->get_style_context()->add_provider(css_provider, 0);

    css_provider->load_from_data(css_data);

    Gtk::Grid::attach(*this->m_header_label, 0, 0, 1, 1);
    Gtk::Grid::attach(*this->m_content_area, 0, 1, 1, 1);

    this->show_all();
}

/**
 * Gets if the current page is valid.
 * @return @c TRUE if the page is valid or @c FALSE otherwise.
 */
bool AssistantPage::is_valid()
{
    return this->validate_page();
}

/**
 * Sets the header text.
 * @param header New header text.
 */
void AssistantPage::set_header(const Glib::ustring &header)
{
    this->m_header_label->set_text(header);
}

/**
 * Gets the header text.
 * @return Current assistant page header text.
 */
Glib::ustring AssistantPage::get_header() const
{
    return this->m_header_label->get_text();
}

/**
 * Gets the content area Gtk::Grid.
 * @return Current assistant page content area.
 */
Gtk::Grid *AssistantPage::get_content_area()
{
    return this->m_content_area;
}

/**
 * Gets the signal emitted when the assistant page is completed.
 * @return Assistant page completed signal.
 */
sigc::signal<void, bool> &AssistantPage::signal_validated()
{
    return this->m_signal_validated;
}

} // ClassCreator
