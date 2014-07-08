/**
 * @file
 * PreferencesDialog class declaration.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP


#include <giomm/settings.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/textview.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Dialog for setting the application's preferences.
 */
class PreferencesDialog final : public Gtk::Dialog
{
private:
    Gtk::Entry *m_author_entry           = nullptr,
               *m_copyright_holder_entry = nullptr,
               *m_date_format_entry      = nullptr,
               *m_header_extension_entry = nullptr,
               *m_source_extension_entry = nullptr;
    Gtk::TextView *m_license_info_tv     = nullptr;
    Gtk::Label  *m_date_example_label    = nullptr;


    Glib::RefPtr<Gio::Settings> m_settings;

    void load_default_settings();
    bool validate_controls() const;
    void on_entry_changed(Gtk::Entry *sender);
    void on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender);

public:
    PreferencesDialog(bool modal = false);
    void apply_settings() const;
};

} // ClassCreator

#endif // PREFERENCESDIALOG_HPP
