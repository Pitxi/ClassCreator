/**
 * @file
 * Implementación de la clase AssistantPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "assistantpage.h"

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Constructor.
 * @param header_text Texto de lacabecera de la página.
 * @param size Tamaño del texto de la cabecera. Si se omite su valor será 24.
 */
AssistantPage::AssistantPage(const Glib::ustring &header_text, int size)
{
    this->m_header_label = Gtk::manage(new Gtk::Label(Glib::ustring(), 0.0, 0.5));
    this->m_content_area = Gtk::manage(new Gtk::Grid);

    this->m_header_label->get_style_context()->add_class("header_label");
    this->setHeaderMargin(50);
    this->m_header_label->set_hexpand();
    this->setPageHeaderText(header_text, size);

    this->m_content_area->set_column_spacing(5);
    this->m_content_area->set_row_spacing(5);

    Grid::attach(*this->m_header_label, 0, 0, 1, 1);
    Grid::attach(*this->m_content_area,0, 1, 1, 1);
}

/**
 * Establece el margen inferior de la cabecera de la página.
 * @param margin Nuevo margen inferior de la cabecera.
 */
void AssistantPage::setHeaderMargin(int margin)
{
    this->m_header_label->set_margin_bottom(margin);
}

/**
 * Establece el texto del título de cabecera de la página y su tamaño.
 * @param header_text Nuevo texto.
 * @param size Tamaño del texto.
 */
void AssistantPage::setPageHeaderText(const Glib::ustring &header_text, int size)
{
    this->m_header_label->set_markup(Glib::ustring::compose("<span font='%1' font_weight='bold'>%2</span>", size, header_text));
}

/**
 * Obtiene el texto del título de cabecera de la página.
 * @return Texto del título de cabecera de la página.
 */
Glib::ustring AssistantPage::getPageHeaderText() const
{
    return this->m_header_label->get_text();
}

/**
 * Añade elementos al área de contenido de la pñagina.
 * @param child Widget que se desea añadir a la página.
 * @param left Columna
 * @param top Fila
 * @param width Anchura en columnas que ocupará el widget.
 * @param height Altura en columnas que ocupará el widget.
 */
void AssistantPage::attach(Gtk::Widget &child, int left, int top, int width, int height)
{
    this->m_content_area->attach(child, left, top, width, height);
}

AssistantPage::~AssistantPage() {}

} // ClassCreator
