/**
 * @file
 * Declaración de la clase DocumentationPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef DOCUMENTATIONPAGE_H
#define DOCUMENTATIONPAGE_H

#include "assistantpage.h"
#include "datepicker.h"
#include <gtkmm/checkbutton.h>
#include <gtkmm/entry.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Página de introducción del asistente.
 */
class DocumentationPage : public AssistantPage
{
private:
    Gtk::CheckButton *m_include_doc_cb;
    Gtk::Entry *m_author_entry, *m_license_entry, *m_format_entry;
    DatePicker *m_date_picker;

protected:
    void updatePage();
    void onEntryIconReleased(Gtk::EntryIconPosition icon_position, const GdkEventButton *event, Gtk::Entry *entry);

public:
    DocumentationPage();
    virtual ~DocumentationPage() {}
    bool getIncludeDocumentation() const;
    void setIncludeDocumentation(bool include);
    Glib::ustring getAuthor() const;
    Glib::ustring getLicense() const;
    Glib::ustring getDateStr() const;
};

} // ClassCreator

#endif // DOCUMENTATIONPAGE_H
