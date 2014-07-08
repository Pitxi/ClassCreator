/**
 * @file
 * Declaración de la clase ComfirmPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include "assistantpage.h"
#include <gtksourceviewmm/view.h>
#include <gtkmm/frame.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Resumen de las operaciones que se aplicarán para crear la nueva clase.
 */
class ConfirmPage : public AssistantPage
{
private:
    Gsv::View *m_header_view, *m_source_view;

public:
    ConfirmPage();
    virtual ~ConfirmPage() {}
    void setHeaderContent(const Glib::ustring &content);
    void setSourceContent(const Glib::ustring &content);
    Glib::ustring getHeaderContent() const;
    Glib::ustring getSourceContent() const;
};

} // ClassCreator

#endif // CONFIRMPAGE_H
