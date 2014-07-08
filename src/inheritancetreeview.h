/**
 * @file
 * Declaración de la clase InheritanceTreeView.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef INHERITANCETREEVIEW_H
#define INHERITANCETREEVIEW_H

#include <gtkmm/frame.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Columnas del modelo para el ComboBox de especificadores de acceso a herencia.
 */
struct SpecifiersColumns : public Gtk::TreeModel::ColumnRecord
{
    Gtk::TreeModelColumn<Glib::ustring> specifier; ///< Columna de especificador de acceso a herencia del ComboBox.

    SpecifiersColumns() {this->add(this->specifier);}
};

/**
 * Columnas del modelo del treeview.
 */
struct BaseClassesColumns : public Gtk::TreeModel::ColumnRecord
{
    Gtk::TreeModelColumn<Glib::ustring> specifier; ///< Columna del especificador de acceso a herencia.
    Gtk::TreeModelColumn<Glib::ustring> classname; ///< Columna de nombre de clase.

    BaseClassesColumns() {this->add(specifier); this->add(classname);}
};

/**
 * Treeview para gestionar las clases bases de la nueva clase.
 */
class InheritanceTreeView : public Gtk::Frame
{
private:
    Gtk::TreeView *m_treeview;
    Gtk::Button *m_add_button, *m_remove_button;
    BaseClassesColumns m_columns;
    SpecifiersColumns m_specifiers_columns;

protected:
    void onSpecifierChanged(const Glib::ustring &path, const Gtk::TreeModel::iterator &iter);
    void onBaseClassEdited(const Glib::ustring &path, const Glib::ustring &new_text);
    void onRowInserted(const Gtk::TreeModel::Path &path, const Gtk::TreeModel::iterator &iter);
    void onSelectionChanged();
    void onAddClicked();
    void onRemoveClicked();

public:
    InheritanceTreeView();
    virtual ~InheritanceTreeView() {}
    Glib::RefPtr<Gtk::ListStore> getModel() const;
};

} // ClassCreator

#endif // INHERITANCETREEVIEW_H
