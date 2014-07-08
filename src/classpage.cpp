/**
 * @file
 * Implementación de la clase AssistantPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "classpage.h"
#include "config.h"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/label.h>
#include <giomm/settings.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Icono del Gtk::Entry pulsado.
 * @param icon_position Posición del icono pulsado.
 * @param event Estructura de evento Gdk de pulsación de botón.
 * @param entry Caja de texto que lanza el evento.
 */
void ClassPage::onEntryIconReleased(Gtk::EntryIconPosition icon_position, const GdkEventButton *event, Gtk::Entry *entry)
{
    if (icon_position == Gtk::EntryIconPosition::ENTRY_ICON_SECONDARY &&
        event->button == GDK_BUTTON_PRIMARY) {
        entry->delete_text(0, entry->get_text().length());
    }
}

/**
 * El Gtk::Entry ha cambiado.
 * @param entry Caja de texto que lanza el evento.
 */
void ClassPage::onEntryChanged(Gtk::Entry *entry)
{
    entry->set_icon_sensitive(Gtk::ENTRY_ICON_SECONDARY, entry->get_text().length() > 0);

    if (entry == this->m_class_name_entry) {
        this->updateFilenames();
    }

    if (!this->m_class_name_entry->get_text().empty() &&
        !this->m_header_file_entry->get_text().empty() &&
        !this->m_source_file_entry->get_text().empty()
       ) {
        this->m_signal_completion_status_changed.emit(true);
    } else {
        this->m_signal_completion_status_changed.emit(false);
    }
}

/**
 * Actualiza los nombres de los ficheros.
 */
void ClassPage::updateFilenames()
{
    auto settings = Gio::Settings::create(APP_ID, APP_PATH);
    Glib::ustring path = this->m_path_fcb->get_filename();
    Glib::ustring base_filename = this->m_class_name_entry->get_text().lowercase();

    this->m_header_file_entry->set_text(Glib::ustring());
    this->m_source_file_entry->set_text(Glib::ustring());

    if (base_filename.length() > 0) {
        this->m_header_file_entry->set_text(Glib::ustring::compose("%1/%2.%3", path, base_filename, settings->get_string("header-ext")));
        this->m_source_file_entry->set_text(Glib::ustring::compose("%1/%2.%3", path, base_filename, settings->get_string("source-ext")));
    }
}

/**
 * Constructor.
 */
ClassPage::ClassPage() :
    AssistantPage(_("Class info"))
{
    Gtk::Label *namespace_label = Gtk::manage(new Gtk::Label(_("Name space:"), 0, 0.5)),
               *class_name_label = Gtk::manage(new Gtk::Label(_("Class name:"), 0, 0.5)),
               *path_label = Gtk::manage(new Gtk::Label(_("Path:"), 0, 0.5)),
               *header_file_label = Gtk::manage(new Gtk::Label(_("Header file:"), 0, 0.5)),
               *source_file_label = Gtk::manage(new Gtk::Label(_("Source file:"), 0, 0.5));
    this->m_namespace_entry = Gtk::manage(new Gtk::Entry);
    this->m_class_name_entry = Gtk::manage(new Gtk::Entry);
    this->m_header_file_entry = Gtk::manage(new Gtk::Entry);
    this->m_source_file_entry = Gtk::manage(new Gtk::Entry);
    this->m_inheritance_treeview = Gtk::manage(new InheritanceTreeView);
    this->m_path_fcb = Gtk::manage(new Gtk::FileChooserButton(_("Header and source files folder"), Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER));

    this->m_class_name_entry->set_icon_from_icon_name("edit-clear", Gtk::ENTRY_ICON_SECONDARY);
    this->m_class_name_entry->set_hexpand();

    this->m_header_file_entry->set_icon_from_icon_name("edit-clear", Gtk::ENTRY_ICON_SECONDARY);
    this->m_header_file_entry->set_hexpand();

    this->m_inheritance_treeview->set_hexpand();
    this->m_inheritance_treeview->set_vexpand();

    this->m_namespace_entry->set_icon_from_icon_name("edit-clear", Gtk::ENTRY_ICON_SECONDARY);
    this->m_namespace_entry->set_hexpand();

    this->m_source_file_entry->set_icon_from_icon_name("edit-clear", Gtk::ENTRY_ICON_SECONDARY);
    this->m_source_file_entry->set_hexpand();

    this->m_path_fcb->set_current_folder(Glib::get_home_dir());
    this->m_path_fcb->select_filename(Glib::get_home_dir());

    this->attach(*namespace_label, 0, 1, 1, 1);
    this->attach(*this->m_namespace_entry, 1, 1, 1, 1);
    this->attach(*class_name_label, 0, 2, 1, 1);
    this->attach(*this->m_class_name_entry, 1, 2, 1, 1);
    this->attach(*this->m_inheritance_treeview, 0, 3, 2, 1);
    this->attach(*path_label, 0, 4, 1, 1);
    this->attach(*this->m_path_fcb, 1, 4, 1, 1);
    this->attach(*header_file_label, 0, 5, 1, 1);
    this->attach(*this->m_header_file_entry, 1, 5, 1, 1);
    this->attach(*source_file_label, 0, 6, 1, 1);
    this->attach(*this->m_source_file_entry, 1, 6, 1, 1);

    for (auto child : this->m_content_area->get_children()) {
        auto entry = dynamic_cast<Gtk::Entry *>(child);
        if (entry != nullptr) {
            this->onEntryChanged(entry);
            entry->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ClassPage::onEntryChanged), entry));
            entry->signal_icon_release().connect(sigc::bind(sigc::mem_fun(*this, &ClassPage::onEntryIconReleased), entry));
        }
    }
    this->m_path_fcb->signal_current_folder_changed().connect(sigc::mem_fun(*this, &ClassPage::updateFilenames));
    this->m_path_fcb->signal_selection_changed().connect(sigc::mem_fun(*this, &ClassPage::updateFilenames));
}

/**
 * Obtiene el nombre del namespace.
 * @return Namespace de la clase.
 */
Glib::ustring ClassPage::getNamespace() const
{
    return this->m_namespace_entry->get_text();
}

/**
 * Obtiene el nombre de la clase.
 * @return Nombre de la casa.
 */
Glib::ustring ClassPage::getClassName() const
{
    return this->m_class_name_entry->get_text();
}

/**
 * Obtiene las clases base.
 * @return Cadena las clases base y sus especificadores de acceso a herencia,
 * tal y como aparecerán en la declaración de la clase.
 */
Glib::ustring ClassPage::getBaseClassesSyntax() const
{
    Glib::ustring inheritance;
    BaseClassesColumns columns;
    Glib::RefPtr<Gtk::ListStore> model = this->m_inheritance_treeview->getModel();
    Gtk::TreeRow row;
    int index = 0;

    if (!model->children().empty()) {
        for (index; index < model->children().size() - 1; ++index) {
            row = model->children()[index];
            inheritance += Glib::ustring::compose("%1 %2, ", row.get_value(columns.specifier).lowercase(), row.get_value(columns.classname));
        }

        row = model->children()[index];
        inheritance += Glib::ustring::compose("%1 %2", row.get_value(columns.specifier).lowercase(), row.get_value(columns.classname));
    }

    return (inheritance.empty() ? inheritance : " : " + inheritance);
}

/**
 * Obtiene el nombre del fichero de cabecera de la clase.
 * @return Fichero de cabecera.
 */
Glib::ustring ClassPage::getHeaderFilename() const
{
    return this->m_header_file_entry->get_text();
}

/**
 * Obtiene el nombre del fichero fuente de la clase.
 * @return Fichero fuente.
 */
Glib::ustring ClassPage::getSourceFilename() const
{
    return this->m_source_file_entry->get_text();
}

} // ClassCreator
