/**
 * @file
 * DocInfoPage class declaration.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#ifndef DOCINFOPAGE_HPP
#define DOCINFOPAGE_HPP

#include "assistantpage.hpp"
#include <giomm/settings.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/entry.h>
#include <gtkmm/textview.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Assistant page for retrieving the class documentation info from the user.
 */
class DocInfoPage final : public AssistantPage
{
private:
    Gtk::CheckButton *m_include_doc_cb             = nullptr;
    Gtk::Entry *m_author_entry                     = nullptr,
               *m_copyright_holder_entry           = nullptr,
               *m_date_format_entry                = nullptr;
    Gtk::TextView *m_license_info_tv               = nullptr;
    Gtk::Button *m_default_author_button           = nullptr,
                *m_default_copyright_holder_button = nullptr,
                *m_default_date_format_button      = nullptr,
                *m_default_license_info_button     = nullptr;
    Gtk::Label  *m_date_example_label              = nullptr;

    Glib::RefPtr<Gio::Settings> m_settings;

    bool validate_page();
    void load_default_values();
    void on_entry_changed(Gtk::Entry *sender);
    void on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender);
    void on_default_button_clicked(Gtk::Button *sender);

public:
    DocInfoPage();

    bool include_documentation() const;
    Glib::ustring get_author() const;
    Glib::ustring get_copyright_holder() const;
    Glib::ustring get_date_format() const;
    Glib::ustring get_license_info() const;
};

} // ClassCreator
#endif // DOCINFOPAGE_HPP
