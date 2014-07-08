/**
 * @file
 * Implementación de la clase InheritanceTreeView.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "inheritancetreeview.h"
#include <glibmm/i18n.h>
#include <gtkmm/grid.h>
#include <gtkmm/cellrenderercombo.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/alignment.h>
#include <gtkmm/scrolledwindow.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Cambia el valor del especificador de acceso a herencia la clase base en el
 * modelo del treeview cuando este cambia en el modelo del CellRendererCombo
 * @param path Una cadena a la ruta que identifica la celda editada (relativa al
 * modelo del TreeView).
 * @param iter El nuevo iterador seleccionado en el combo box (relativo al
 * modelo del combo box).
 */
void InheritanceTreeView::onSpecifierChanged(const Glib::ustring &path, const Gtk::TreeModel::iterator &iter)
{
    Gtk::TreeRow row = *this->getModel()->get_iter(path);
    row[this->m_columns.specifier] = iter->get_value(this->m_specifiers_columns.specifier);
}

/**
 * Cambia el valor del nombre de la clase base una vez éste ha sido editado.
 * @param path La ruta que identifica la celda editada.
 * @param new_text El nuevo texto.
 */
void InheritanceTreeView::onBaseClassEdited(const Glib::ustring &path, const Glib::ustring &new_text)
{
    Gtk::TreeIter iter = this->getModel()->get_iter(path);
    Gtk::TreeRow row = *iter;

    row[this->m_columns.classname] = new_text;

    if (new_text.empty()) {
        this->getModel()->erase(iter);
    }
}

/**
 * Selecciona automáticamente la nueva fila insertada y establece los valores por defecto;
 * @param path Ruta que identifica la nueva fila.
 * @param iter Iterador a la nueva fila.
 */
void InheritanceTreeView::onRowInserted(const Gtk::TreeModel::Path &path, const Gtk::TreeModel::iterator &iter)
{
    Gtk::TreeRow row = *iter;
    Gtk::CellRendererCombo *specifiers_cell_renderer = dynamic_cast<Gtk::CellRendererCombo *>(this->m_treeview->get_column_cell_renderer(0));
    Glib::RefPtr<Gtk::ListStore> specifiers_lstore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(specifiers_cell_renderer->property_model().get_value());

    row[this->m_columns.specifier] = specifiers_lstore->children().begin()->get_value(this->m_specifiers_columns.specifier);
    this->m_treeview->set_cursor(path, *this->m_treeview->get_column(1), true);
}

/**
 * Cambia la sensibilidad del botón para eliminar clases base si no hay ninguna
 * seleccionada.
 */
void InheritanceTreeView::onSelectionChanged()
{
    Gtk::TreeIter iter = this->m_treeview->get_selection()->get_selected();
    this->m_remove_button->set_sensitive(this->getModel()->iter_is_valid(iter));
}

/**
 * Añade una nueva fila al treeview y comienza a editar el nombre de la nueva
 * clase base.
 */
void InheritanceTreeView::onAddClicked()
{
    this->getModel()->append();
}

/**
 * Elimina la clase base seleccionada.
 */
void InheritanceTreeView::onRemoveClicked()
{
    Gtk::TreeIter iter = this->m_treeview->get_selection()->get_selected();
    this->getModel()->erase(iter);
}

/**
 * Constructor.
 */
InheritanceTreeView::InheritanceTreeView() :
    Gtk::Frame(_("Base classes"))
{
    Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
    Gtk::ScrolledWindow *sw = Gtk::manage(new Gtk::ScrolledWindow);
    Gtk::ButtonBox *bbox = Gtk::manage(new Gtk::ButtonBox);
    Glib::RefPtr<Gtk::ListStore> base_classes_lstore = Gtk::ListStore::create(this->m_columns),
                                 specifiers_lstore = Gtk::ListStore::create(this->m_specifiers_columns);
    Gtk::CellRendererCombo *specifier_cell_renderer = Gtk::manage(new Gtk::CellRendererCombo);
    Gtk::CellRendererText *classname_cell_renderer = Gtk::manage(new Gtk::CellRendererText);
    Gtk::TreeViewColumn *specifier_column = Gtk::manage(new Gtk::TreeViewColumn(_("Specifier"), *specifier_cell_renderer));
    Gtk::TreeViewColumn *classname_column = Gtk::manage(new Gtk::TreeViewColumn(_("Class name"), *classname_cell_renderer));
    this->m_treeview = Gtk::manage(new Gtk::TreeView(base_classes_lstore));
    this->m_add_button = Gtk::manage(new Gtk::Button),
    this->m_remove_button = Gtk::manage(new Gtk::Button);

    (*specifiers_lstore->append())[this->m_specifiers_columns.specifier] = "Public";
    (*specifiers_lstore->append())[this->m_specifiers_columns.specifier] = "Protected";
    (*specifiers_lstore->append())[this->m_specifiers_columns.specifier] = "Private";

    specifier_cell_renderer->property_model() = specifiers_lstore;
    specifier_cell_renderer->property_editable() = true;
    specifier_cell_renderer->property_text_column() = 0;
    specifier_cell_renderer->property_has_entry() = false;

    classname_cell_renderer->property_editable() = true;

    specifier_column->add_attribute(*specifier_cell_renderer, "text", this->m_columns.specifier);

    classname_column->add_attribute(*classname_cell_renderer, "text", this->m_columns.classname);

    this->m_treeview->set_hexpand();
    this->m_treeview->set_vexpand();
    this->m_treeview->set_rules_hint();
    this->m_treeview->append_column(*specifier_column);
    this->m_treeview->append_column(*classname_column);
    specifier_column->get_button()->set_tooltip_text(_("Inheritance access specifier"));
    classname_column->get_button()->set_tooltip_text(_("Base class name"));

    sw->add(*this->m_treeview);

    this->m_add_button->set_image_from_icon_name("list-add");
    this->m_add_button->set_tooltip_text(_("Add a new base class"));

    this->m_remove_button->set_image_from_icon_name("list-remove");
    this->m_remove_button->set_tooltip_text(_("Remove selected base class"));
    this->m_remove_button->set_sensitive(false);

    bbox->pack_start(*this->m_add_button, false, false, 5);
    bbox->pack_start(*this->m_remove_button, false, false, 5);
    bbox->set_halign(Gtk::ALIGN_START);

    grid->set_column_spacing(5);
    grid->set_row_spacing(5);
    grid->set_margin_bottom(5);
    grid->set_margin_left(5);
    grid->set_margin_right(5);
    grid->set_margin_top(5);
    grid->attach(*bbox, 0, 0, 1, 1);
    grid->attach(*sw, 0, 1, 1, 1);

    sw->set_size_request(-1, 150);

    this->add(*grid);

    specifier_cell_renderer->signal_changed().connect(sigc::mem_fun(*this, &InheritanceTreeView::onSpecifierChanged));
    classname_cell_renderer->signal_edited().connect(sigc::mem_fun(*this, &InheritanceTreeView::onBaseClassEdited));
    base_classes_lstore->signal_row_inserted().connect(sigc::mem_fun(*this, &InheritanceTreeView::onRowInserted));
    this->m_add_button->signal_clicked().connect(sigc::mem_fun(*this, &InheritanceTreeView::onAddClicked));
    this->m_remove_button->signal_clicked().connect(sigc::mem_fun(*this, &InheritanceTreeView::onRemoveClicked));
    this->m_treeview->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &InheritanceTreeView::onSelectionChanged));
}

/**
 * Obtiene el modelo del TreeView en el que se almacenan las clases bases.
 * @return ListStore con las clases bases.
 */
Glib::RefPtr<Gtk::ListStore> InheritanceTreeView::getModel() const
{
    return Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->m_treeview->get_model());
}

} // ClassCreator
