/**
 * @file
 * Implementación de la clase SummaryPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "confirmpage.h"
#include <glibmm/i18n.h>
#include <gtkmm/scrolledwindow.h>
#include <gtksourceviewmm/init.h>
#include <gtksourceviewmm/languagemanager.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Constructor.
 */
ConfirmPage::ConfirmPage() : AssistantPage(_("Preview and confirmation"))
{
    Gsv::init();

    auto language_manager = Gsv::LanguageManager::create();
    auto header_sw = Gtk::manage(new Gtk::ScrolledWindow),
         source_sw = Gtk::manage(new Gtk::ScrolledWindow);
    auto header_frame = Gtk::manage(new Gtk::Frame(_("C++ header file"))),
         source_frame = Gtk::manage(new Gtk::Frame(_("C++ source file")));
    this->m_header_view = Gtk::manage(new Gsv::View);
    this->m_source_view = Gtk::manage(new Gsv::View);

    this->m_header_view->set_show_line_numbers();
    this->m_header_view->set_hexpand();
    this->m_header_view->set_vexpand();
    this->m_header_view->set_source_buffer(Gsv::Buffer::create());
    this->m_header_view->get_source_buffer()->set_language(language_manager->get_language("cpp"));
    this->m_header_view->set_editable(false);
    this->m_header_view->set_margin_bottom(5);
    this->m_header_view->set_margin_left(5);
    this->m_header_view->set_margin_right(5);
    this->m_header_view->set_margin_top(5);
    header_sw->add(*this->m_header_view);
    header_frame->add(*header_sw);

    header_frame->set_margin_bottom(25);

    this->m_source_view->set_show_line_numbers();
    this->m_source_view->set_hexpand();
    this->m_source_view->set_vexpand();
    this->m_source_view->set_source_buffer(Gsv::Buffer::create());
    this->m_source_view->get_source_buffer()->set_language(language_manager->get_language("cpp"));
    this->m_source_view->set_editable(false);
    this->m_source_view->set_margin_bottom(5);
    this->m_source_view->set_margin_left(5);
    this->m_source_view->set_margin_right(5);
    this->m_source_view->set_margin_top(5);
    source_sw->add(*this->m_source_view);
    source_frame->add(*source_sw);

    this->attach(*header_frame, 0, 0, 1, 1);
    this->attach(*source_frame, 0, 1, 1, 1);
}

/**
 * Establece el contenido de la vista previa del fichero de cabecera.
 * @param content Contenido del fichero de cabecera.
 */
void ConfirmPage::setHeaderContent(const Glib::ustring &content)
{
    this->m_header_view->get_buffer()->set_text(content);
}

/**
 * Establece el contenido de la vista previa del fichero fuente.
 * @param content Contenido del fichero fuente.
 */
void ConfirmPage::setSourceContent(const Glib::ustring &content)
{
    this->m_source_view->get_buffer()->set_text(content);
}

/**
 * Obtiene el contenido del fichero de cabecera.
 * @return Contenido del fichero de cabecera.
 */
Glib::ustring ConfirmPage::getHeaderContent() const
{
    return this->m_header_view->get_buffer()->get_text();
}

/**
 * Obtiene el contenido del fichero fuente.
 * @return Contenido del fichero fuente.
 */
Glib::ustring ConfirmPage::getSourceContent() const
{
    return this->m_source_view->get_buffer()->get_text();
}

} // ClassCreator
