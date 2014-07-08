/**
 * @file
 * Declaración de la clase ClassCreatorAssistant.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef CLASSCREATORASSISTANT_H
#define CLASSCREATORASSISTANT_H

#include <gtkmm/assistant.h>
#include <gtkmm/filechooserbutton.h>
#include "assistantpage.h"
#include "intropage.h"
#include "classpage.h"
#include "documentationpage.h"
#include "confirmpage.h"

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Asistente para la creación de los ficheros de cabecera y fuente de una clase
 * C++.
 */
class ClassCreatorAssistant : public Gtk::Assistant
{
private:
    Gtk::FileChooserButton *m_fcb_header, *m_fcb_source;
    IntroPage *m_intro_page;
    ClassPage *m_class_page;
    DocumentationPage *m_documentation_page;
    ConfirmPage *m_confirm_page;

    Glib::ustring parse(Glib::ustring template_str) const;
    void parseSource() const;

protected:
    virtual void on_hide();
    void onPrepare(Gtk::Widget *page);
    void onPageCompletionStatusChanged(bool completed, Gtk::Widget *page);
    void onApply();

public:
    ClassCreatorAssistant(const Glib::ustring &title);
    virtual ~ClassCreatorAssistant() {}
};

} // CPPTest

#endif // CLASSCREATORASSISTANT_H
