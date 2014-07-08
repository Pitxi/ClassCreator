/**
 * @file
 * Implementación de la clase AssistantPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "documentationpage.h"
#include "config.h"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <giomm/settings.h>
#include <gtkmm/label.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Actualiza el estado de la página segun el contenido de las cajas de texto.
 */
void DocumentationPage::updatePage()
{
    bool sensitive = this->m_include_doc_cb->get_active();
    Glib::ustring author = this->m_author_entry->get_text(),
                  license = this->m_license_entry->get_text(),
                  date_format = this->m_format_entry->get_text(),
                  date_str;

    this->m_author_entry->set_sensitive(sensitive);
    this->m_date_picker->set_sensitive(sensitive);
    this->m_format_entry->set_sensitive(sensitive);
    this->m_license_entry->set_sensitive(sensitive);
    this->m_date_picker->setFormat(this->m_format_entry->get_text());
    date_str = this->m_date_picker->getDate().format_string(date_format);

    this->m_signal_completion_status_changed.emit(!sensitive || (!author.empty() && !license.empty() && !date_str.empty()));
}

/**
 * Icono del Gtk::Entry pulsado.
 * @param icon_position Posición del icono pulsado.
 * @param event Estructura de evento Gdk de pulsación de botón.
 * @param entry Caja de texto que lanza el evento.
 */
void DocumentationPage::onEntryIconReleased(Gtk::EntryIconPosition icon_position, const GdkEventButton *event, Gtk::Entry *entry)
{
    if (icon_position == Gtk::EntryIconPosition::ENTRY_ICON_SECONDARY &&
        event->button == GDK_BUTTON_PRIMARY) {
        entry->delete_text(0, entry->get_text().length());
    }
}

/**
 * Constructor.
 */
DocumentationPage::DocumentationPage() :
    AssistantPage(gettext("Documentation info"))
{
    auto settings = Gio::Settings::create(APP_ID, APP_PATH);
    auto author_label = Gtk::manage(new Gtk::Label(gettext("Author:"), 0, 0.5));
    auto date_label = Gtk::manage(new Gtk::Label(gettext("Date:"), 0, 0.5));
    auto license_label = Gtk::manage(new Gtk::Label(gettext("License:"), 0, 0.5));
    auto format_label = Gtk::manage(new Gtk::Label(gettext("Date format:"), 0, 0.5));
    this->m_format_entry = Gtk::manage(new Gtk::Entry);
    this->m_include_doc_cb = Gtk::manage(new Gtk::CheckButton(gettext("Include documentation")));
    this->m_author_entry = Gtk::manage(new Gtk::Entry);
    this->m_date_picker = Gtk::manage(new DatePicker);
    this->m_license_entry = Gtk::manage(new Gtk::Entry);

    this->attach(*this->m_include_doc_cb, 0, 0, 2, 1);
    this->attach(*author_label, 0, 1, 1, 1);
    this->attach(*this->m_author_entry, 1, 1, 1, 1);
    this->attach(*date_label, 0, 2, 1, 1);
    this->attach(*this->m_date_picker, 1, 2, 1, 1);
    this->attach(*format_label, 0, 3, 1, 1);
    this->attach(*this->m_format_entry, 1, 3, 1, 1);
    this->attach(*license_label, 0, 4, 1, 1);
    this->attach(*this->m_license_entry, 1, 4, 1, 1);

    this->m_author_entry->set_text(settings->get_string("author"));
    this->m_author_entry->set_hexpand();

    this->m_date_picker->set_hexpand();

    this->m_format_entry->set_text(settings->get_string("date-format"));
    this->m_format_entry->set_hexpand();

    this->m_license_entry->set_text(settings->get_string("license"));
    this->m_license_entry->set_hexpand();

    this->setHeaderMargin(150);

    for (auto child : this->m_content_area->get_children()) {
        auto entry = dynamic_cast<Gtk::Entry *>(child);
        if (entry != nullptr) {
            entry->set_icon_from_icon_name("edit-clear", Gtk::ENTRY_ICON_SECONDARY);
            entry->signal_changed().connect(sigc::mem_fun(*this, &DocumentationPage::updatePage));
            entry->signal_icon_release().connect(sigc::bind(sigc::mem_fun(*this, &DocumentationPage::onEntryIconReleased), entry));
        }
    }
    this->m_include_doc_cb->signal_toggled().connect(sigc::mem_fun(*this, &DocumentationPage::updatePage));
    this->updatePage();
}

/**
 * Establece si se incluirá documentación de código por defecto al crear la
 * nueva clase.
 * @return @c TRUE si debe incluirse la documentación o @c FALSE en caso
 * contrario.
 */
bool DocumentationPage::getIncludeDocumentation() const
{
    return this->m_include_doc_cb->get_active();
}

/**
 * Establece si se incluirá documentación de código por defecto al crear la
 * nueva clase.
 * @param include @c TRUE para incluir la documentación o @c FALSE en caso
 * contrario.
 */
void DocumentationPage::setIncludeDocumentation(bool include)
{
    this->m_include_doc_cb->set_active(include);
}

/**
 * Obtiene el autor para la adocumentación de la clase.
 * @return Autor de la clase.
 */
Glib::ustring DocumentationPage::getAuthor() const
{
    return this->m_author_entry->get_text();
}

/**
 * Obtiene la licencia bajo la que se distribuye el código de la clase.
 * @return Licencia del código.
 */
Glib::ustring DocumentationPage::getLicense() const
{
    return this->m_license_entry->get_text();
}

/**
 * Obtiene una cadena con la fecha de creación de la clase formateada con el
 * formato establecido por el usuario.
 * @return Fecha de creación de la clase formateada.
 */
Glib::ustring DocumentationPage::getDateStr() const
{
    return this->m_date_picker->getDate().format_string(this->m_format_entry->get_text());
}

} // ClassCreator
