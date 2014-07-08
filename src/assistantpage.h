/**
 * @file
 * Declaración de la clase AssistantPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef ASSISTANTPAGE_H
#define ASSISTANTPAGE_H

#include <gtkmm/grid.h>
#include <gtkmm/label.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Página de introducción del asistente.
 */
class AssistantPage : public Gtk::Grid
{
protected:
    sigc::signal<void, bool> m_signal_completion_status_changed; ///< Señal lanzada al cambiar el estado completado de la pñagina de asistente.
    Gtk::Label *m_header_label; ///< Etiqueta con el texto de cabecera de la página de asistente.
    Gtk::Grid *m_content_area; ///< Área de contenidos de la página de asistente.

public:
    sigc::signal<void, bool> &signal_page_completion_status_changed() {return this->m_signal_completion_status_changed;} ///< Página del sistente completada.

    AssistantPage(const Glib::ustring &header_text, int size = 24);
    virtual ~AssistantPage() = 0;
    void setHeaderMargin(int margin);
    void setPageHeaderText(const Glib::ustring &header_text, int size = 24);
    Glib::ustring getPageHeaderText() const;
    void attach(Widget &child, int left, int top, int width, int height);
};

} // ClassCreator

#endif // ASSISTANTPAGE_H
