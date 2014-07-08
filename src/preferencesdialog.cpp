/**
 * @file
 * Implementación de la clase PreferenceDialog.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */
#include "preferencesdialog.h"
#include "config.h"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/grid.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Actualiza la sensibilidad del botón aceptar segun los valores de los
 * controles.
 */
void PreferencesDialog::updateButtons()
{
    Gtk::Button *accept_button = dynamic_cast<Gtk::Button *>(this->get_widget_for_response(Gtk::RESPONSE_ACCEPT));

    accept_button->set_sensitive(!this->getAuthor().empty() &&
                                 !this->getDateFormat().empty() &&
                                 !this->getLicense().empty() &&
                                 !this->getHeaderExtension().empty() &&
                                 !this->getSourceExtension().empty());
}

/**
 * Icono del Gtk::Entry pulsado.
 * @param icon_position Posición del icono pulsado.
 * @param event Estructura de evento Gdk de pulsación de botón.
 * @param entry Caja de texto que lanza el evento.
 */
void PreferencesDialog::onEntryIconReleased(Gtk::EntryIconPosition icon_position, const GdkEventButton *event, Gtk::Entry *entry)
{
    if (icon_position == Gtk::EntryIconPosition::ENTRY_ICON_SECONDARY &&
        event->button == GDK_BUTTON_PRIMARY) {
        entry->delete_text(0, entry->get_text().length());
    } else if (entry == this->m_def_author_entry) {
        entry->set_text(Glib::get_real_name());
    }
}

/**
 * Recupera los valores a su estado previo.
 */
void PreferencesDialog::onUndoClicked()
{
    this->m_settings->revert();
}

/**
 * Constructor.
 */
PreferencesDialog::PreferencesDialog() :
    Gtk::Dialog(_("Preferences"), true)
{
    auto grid = Gtk::manage(new Gtk::Grid);
    auto *def_author_label = Gtk::manage(new Gtk::Label(_("Default author:"), 0, 0.5));
    auto *def_date_format_label = Gtk::manage(new Gtk::Label(_("Date format:"), 0, 0.5));
    auto *def_license_label = Gtk::manage(new Gtk::Label(_("Default license:"), 0, 0.5));
    auto *def_header_ext_label = Gtk::manage(new Gtk::Label(_("Header file extension:"), 0, 0.5));
    auto *def_source_ext_label = Gtk::manage(new Gtk::Label(_("Source file extension:"), 0, 0.5));
    auto *accept_button = this->add_button(_("_Accept"), Gtk::RESPONSE_ACCEPT);
    auto *cancel_button = this->add_button(_("_Cancel"), Gtk::RESPONSE_CANCEL);
    auto *undo_button = Gtk::manage(new Gtk::Button(_("_Undo"), true));
    this->m_settings = Gio::Settings::create(APP_ID, APP_PATH);
    this->m_def_author_entry = Gtk::manage(new Gtk::Entry);
    this->m_def_date_format_entry = Gtk::manage(new Gtk::Entry);
    this->m_def_license_entry = Gtk::manage(new Gtk::Entry);
    this->m_def_header_ext_cbt = Gtk::manage(new Gtk::ComboBoxText(true));
    this->m_def_source_ext_cbt = Gtk::manage(new Gtk::ComboBoxText(true));

    accept_button->set_image_from_icon_name("gtk-apply");

    cancel_button->set_image_from_icon_name("gtk-cancel");

    undo_button->set_image_from_icon_name("document-revert");

    this->m_def_author_entry->set_hexpand();
    this->m_def_author_entry->set_icon_from_icon_name("edit-find", Gtk::ENTRY_ICON_PRIMARY);

    this->m_def_date_format_entry->set_hexpand();

    this->m_def_license_entry->set_hexpand();

    this->m_def_header_ext_cbt->set_hexpand();
    this->m_def_header_ext_cbt->append("h");
    this->m_def_header_ext_cbt->append("hpp");
    this->m_def_header_ext_cbt->append("hxx");

    this->m_def_source_ext_cbt->set_hexpand();
    this->m_def_source_ext_cbt->append("c");
    this->m_def_source_ext_cbt->append("cpp");
    this->m_def_source_ext_cbt->append("cxx");

    this->m_settings->bind("author", this->m_def_author_entry->property_text());
    this->m_settings->bind("date-format", this->m_def_date_format_entry->property_text());
    this->m_settings->bind("license", this->m_def_license_entry->property_text());
    this->m_settings->bind("header-ext", this->m_def_header_ext_cbt->get_entry()->property_text());
    this->m_settings->bind("source-ext", this->m_def_source_ext_cbt->get_entry()->property_text());
    if (this->m_def_author_entry->get_text().empty()) {
        this->m_def_author_entry->set_text(Glib::get_real_name());
    }
    this->m_settings->delay();

    grid->set_column_spacing(5);
    grid->set_row_spacing(5);
    grid->set_margin_bottom(5);
    grid->set_margin_left(5);
    grid->set_margin_right(5);
    grid->set_margin_top(5);
    grid->set_column_spacing(5);
    grid->set_row_spacing(5);
    grid->attach(*def_author_label, 0, 0, 1, 1);
    grid->attach(*this->m_def_author_entry, 1, 0, 1, 1);
    grid->attach(*def_date_format_label, 0, 1, 1, 1);
    grid->attach(*this->m_def_date_format_entry, 1, 1, 1, 1);
    grid->attach(*def_license_label, 0, 2, 1, 1);
    grid->attach(*this->m_def_license_entry, 1, 2, 1, 1);
    grid->attach(*def_header_ext_label, 0, 3, 1, 1);
    grid->attach(*this->m_def_header_ext_cbt, 1, 3, 1, 1);
    grid->attach(*def_source_ext_label, 0, 4, 1, 1);
    grid->attach(*this->m_def_source_ext_cbt, 1, 4, 1, 1);

    this->get_action_area()->pack_end(*undo_button);
    this->get_action_area()->reorder_child(*undo_button, 0);
    this->set_size_request(500, -1);
    this->set_resizable(false);
    this->set_icon_name("preferences-system");
    this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
    this->get_content_area()->pack_start(*grid);
    this->show_all();

    for (auto child : grid->get_children()) {
        auto entry = dynamic_cast<Gtk::Entry *>(child);
        if (entry != nullptr) {
            entry->set_icon_from_icon_name("edit-clear", Gtk::ENTRY_ICON_SECONDARY);
            entry->signal_changed().connect(sigc::mem_fun(*this, &PreferencesDialog::updateButtons));
            entry->signal_icon_release().connect(sigc::bind(sigc::mem_fun(*this, &PreferencesDialog::onEntryIconReleased), entry));
        } else {
            auto cbt = dynamic_cast<Gtk::ComboBoxText *>(child);
            if (cbt != nullptr) {
                cbt->signal_changed().connect(sigc::mem_fun(*this, &PreferencesDialog::updateButtons));
            }
        }
    }
    undo_button->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesDialog::onUndoClicked));
}

/**
 * Obtiene el autor por defecto para la documentación de las nuevas clases.
 * @return Autor por defecto.
 */
Glib::ustring PreferencesDialog::getAuthor() const
{
    return this->m_def_author_entry->get_text();
}

/**
 * Obtiene el formato de fecha, tal como las que usa la función @c strftime.
 * Esta cadena define cómo aparecerá el campo fecha en la documentación de la
 * nueva clase.
 * @return Cadena de formato de fecha.
 */
Glib::ustring PreferencesDialog::getDateFormat() const
{
    return this->m_def_date_format_entry->get_text();
}

/**
 * Obtiene la licencia que se usará por defecto para las nuevas clases.
 * @return Licencia por defecto.
 */
Glib::ustring PreferencesDialog::getLicense() const
{
    return this->m_def_license_entry->get_text();
}

/**
 * Obtiene la extensión del fichero de cabecera de la nueva clase que se usará
 * por defecto.
 * @return Extensión del fichero de cabecera.
 */
Glib::ustring PreferencesDialog::getHeaderExtension() const
{
    return this->m_def_header_ext_cbt->get_entry_text();
}

/**
 * Obtiene la extensión del fichero fuente de la nueva clase que se usará por
 * defecto.
 * @return Extensión del fichero fuente.
 */
Glib::ustring PreferencesDialog::getSourceExtension() const
{
    return this->m_def_source_ext_cbt->get_entry_text();
}

/**
 * Aplica los cambios en la configuración de la aplicación.
 */
void PreferencesDialog::apply() const
{
    this->m_settings->apply();
}

} // ClassCreator
