/**
 * @file
 * Declaración de la clase ClassPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef CLASSPAGE_H
#define CLASSPAGE_H

#include "assistantpage.h"
#include "inheritancetreeview.h"
#include <gtkmm/entry.h>
#include <gtkmm/filechooserbutton.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Página de introducción del asistente.
 */
class ClassPage : public AssistantPage
{
private:
    Gtk::Entry *m_namespace_entry,
               *m_class_name_entry,
               *m_header_file_entry,
               *m_source_file_entry;
    Gtk::FileChooserButton *m_path_fcb;
    InheritanceTreeView *m_inheritance_treeview;

protected:
    void onEntryIconReleased(Gtk::EntryIconPosition icon_position, const GdkEventButton *event, Gtk::Entry *entry);
    void onEntryChanged(Gtk::Entry *entry);
    void updateFilenames();

public:
    ClassPage();
    virtual ~ClassPage() {}
    Glib::ustring getNamespace() const;
    Glib::ustring getClassName() const;
    Glib::ustring getBaseClassesSyntax() const;
    Glib::ustring getHeaderFilename() const;
    Glib::ustring getSourceFilename() const;
};

} // ClassCreator

#endif // CLASSPAGE_H
