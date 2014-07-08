/**
 * @file
 * Declaración de la clase PreferencesDialog.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <giomm/settings.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/comboboxtext.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Diálogo para modificar las preferencias de la aplicación.
 */
class PreferencesDialog : public Gtk::Dialog
{
private:
    Gtk::Entry *m_def_author_entry, *m_def_date_format_entry, *m_def_license_entry;
    Gtk::ComboBoxText *m_def_header_ext_cbt, *m_def_source_ext_cbt;
    Glib::RefPtr<Gio::Settings> m_settings;

protected:
    void updateButtons();
    void onEntryIconReleased(Gtk::EntryIconPosition icon_position, const GdkEventButton *event, Gtk::Entry *entry);
    void onUndoClicked();

public:
    PreferencesDialog();
    virtual ~PreferencesDialog() {}
    Glib::ustring getAuthor() const;
    Glib::ustring getDateFormat() const;
    Glib::ustring getLicense() const;
    Glib::ustring getHeaderExtension() const;
    Glib::ustring getSourceExtension() const;
    void apply() const;
};

} // ClassCreator

#endif // PREFERENCESDIALOG_H
