/**
 * @file
 * AssistantPage class declaration.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#ifndef ASSISTANTPAGE_HPP
#define ASSISTANTPAGE_HPP

#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/assistant.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Abstract class for the creation of Gtk::Assistant page widgets.
 */
class AssistantPage : public Gtk::Grid
{
protected:
    Gtk::Grid  *m_content_area = nullptr;
    Gtk::Label *m_header_label = nullptr;
    sigc::signal<void, bool> m_signal_validated;

    virtual bool validate_page() = 0; ///< Validates the page.

public:
    AssistantPage(const Glib::ustring &header);
    virtual ~AssistantPage() {}

    bool is_valid();
    void set_header(const Glib::ustring &header);
    Glib::ustring get_header() const;
    Gtk::Grid *get_content_area();
    sigc::signal<void, bool> &signal_validated();
};

} // ClassCreator

#endif // ASSISTANTPAGE_HPP
