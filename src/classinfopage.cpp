/**
 * @file
 * ClassInfoPage class implementation.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#include "classinfopage.hpp"
#include "config.h"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/builder.h>
#include <gtkmm/liststore.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Adds a new row to the related TreeView.
 * @param sender The TreeView which emmited the signal.
 */
void ClassInfoPage::on_add_tb_clicked(Gtk::TreeView *sender)
{
    auto model = Glib::RefPtr<Gtk::ListStore>::cast_static(sender->get_model());
    Gtk::TreeIter iter = model->append();
    Glib::ustring default_value1,
                  default_value2;

    auto cell_renderer_combo = static_cast<Gtk::CellRendererCombo*>(sender->get_column_cell_renderer(1));
    auto combo_model = Glib::RefPtr<Gtk::ListStore>::cast_static(cell_renderer_combo->property_model().get_value());
    auto combo_first_iter = combo_model->children().begin();

    combo_first_iter->get_value(0, default_value1);
    combo_first_iter->get_value(1, default_value2);

    iter->set_value(1, default_value1);
    iter->set_value(2, default_value2);
    sender->set_cursor(model->get_path(iter), *sender->get_column(0), true);
}

/**
 * Removes the selected rows from the related TreeView.
 * @param sender The TreeView which emmited the signal.
 */
void ClassInfoPage::on_remove_tb_clicked(Gtk::TreeView *sender)
{
    auto selected_rows = sender->get_selection()->get_selected_rows();
    auto model = Glib::RefPtr<Gtk::ListStore>::cast_static(sender->get_model());
    std::vector<Gtk::TreeIter> selected_iters;

    for (auto row : selected_rows) {
        selected_iters.push_back(model->get_iter(row));
    }

    for (auto iter : selected_iters) {
        model->erase(iter);
    }
}

/**
 * Sets the data in the TreeView model after editing the cell renderer.
 * @param path The path identifying the edited cell.
 * @param new_text The new text.
 * @param tree_view The Related TreeView.
 */
void ClassInfoPage::on_include_file_crt_edited(const Glib::ustring &path, const Glib::ustring &new_text, Gtk::TreeView *tree_view)
{
    auto model = Glib::RefPtr<Gtk::ListStore>::cast_static(tree_view->get_model());
    model->get_iter(path)->set_value(0, new_text);
}

/**
 * Sets the data in the TreeView model after changing the selection on the combo
 * box.
 * @param path The path identifying the edited cell.
 * @param iter The iter selected in the combo box (relative to the combo box
 * model).
 * @param tree_view The Related TreeView.
 */
void ClassInfoPage::on_cell_renderer_combo_changed(const Glib::ustring &path, const Gtk::TreeIter &iter, Gtk::TreeView *tree_view)
{
    auto model = Glib::RefPtr<Gtk::ListStore>::cast_static(tree_view->get_model());
    auto model_iter = model->get_iter(path);
    Glib::ustring value1, value2;


    iter->get_value(0, value1);
    iter->get_value(1, value2);

    model_iter->set_value(1, value1);
    model_iter->set_value(2, value2);
}

/**
 * Sets the sensitivity of the remove ToolButton accordingly to the number of
 * rows selected.
 * @param sender The TreeSelection which emmited the signal.
 */
void ClassInfoPage::on_selection_changed(Gtk::TreeSelection *sender)
{
    auto selected_rows = sender->get_selected_rows().size();
    auto tool_button = this->m_remove_include_tb;

    if (sender->get_tree_view() == this->m_bases_tv) {
        tool_button = this->m_remove_base_tb;
    }

    tool_button->set_sensitive(selected_rows > 0);
}

/**
 * Sets sender's clear text icon sensitivity accordingly to the contents
 * of the sender entry and validates the page when needed.
 * @param sender The entry that emitted the signal.
 */
void ClassInfoPage::on_entry_changed(Gtk::Entry *sender)
{
    sender->set_icon_sensitive(Gtk::ENTRY_ICON_SECONDARY, !sender->get_text().empty());
    if (sender == this->m_class_name_entry) {
        this->validate_page();
    }
}

/**
 * Clears sender's text when it's clear text icon is released.
 * @param icon_pos The position of the released icon.
 * @param event The button release event.
 * @param sender The entry that emitted the signal.
 */
void ClassInfoPage::on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender)
{
    if (icon_pos == Gtk::ENTRY_ICON_SECONDARY && event->button == GDK_BUTTON_PRIMARY) {
        sender->set_text(Glib::ustring());
    }
}

/**
 * Validates the current page and emit the completed signal if the page is
 * valid.
 * @return @c TRUE if the page is valid or @c FALSE otherwise.
 */
bool ClassInfoPage::validate_page()
{
    bool is_valid = !this->m_class_name_entry->get_text().empty();

    this->signal_validated().emit(is_valid);

    return is_valid;
}

/**
 * Constructor.
 */
ClassInfoPage::ClassInfoPage() : AssistantPage(_("New C++ Class information"))
{
    Gtk::Grid *class_info_grid = nullptr;
    auto gui_res_path = Glib::build_filename(APP_PATH, "resources", "gui", "classinfogrid.glade");
    auto builder = Gtk::Builder::create_from_resource(gui_res_path);

    builder->get_widget("ClassInfoGrid",   class_info_grid);
    builder->get_widget("AddIncludeTB",    this->m_add_include_tb);
    builder->get_widget("RemoveIncludeTB", this->m_remove_include_tb);
    builder->get_widget("AddBaseTB",       this->m_add_base_tb);
    builder->get_widget("RemoveBaseTB",    this->m_remove_base_tb);
    builder->get_widget("IncludesTV",      this->m_includes_tv);
    builder->get_widget("BasesTV",         this->m_bases_tv);
    builder->get_widget("ClassNameEntry",  this->m_class_name_entry);
    builder->get_widget("NamespaceEntry",  this->m_namespace_entry);

    auto include_file_crt         = Glib::RefPtr<Gtk::CellRendererText>::cast_static(builder->get_object("IncludeFileCRT")),
         base_class_name_crt      = Glib::RefPtr<Gtk::CellRendererText>::cast_static(builder->get_object("BaseClassNameCRT"));
    auto include_delimiter_crc    = Glib::RefPtr<Gtk::CellRendererCombo>::cast_static(builder->get_object("IncludeDelimiterCRC")),
         inheritance_type_crc     = Glib::RefPtr<Gtk::CellRendererCombo>::cast_static(builder->get_object("InheritanceTypeCRC"));

    this->m_header_label->set_margin_bottom(50);

    this->get_content_area()->attach(*class_info_grid, 0, 0, 1, 1);

    // Signals
    this->m_add_include_tb->signal_clicked().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_add_tb_clicked), this->m_includes_tv));
    this->m_add_base_tb->signal_clicked().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_add_tb_clicked), this->m_bases_tv));
    this->m_remove_include_tb->signal_clicked().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_remove_tb_clicked), this->m_includes_tv));
    this->m_remove_base_tb->signal_clicked().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_remove_tb_clicked), this->m_bases_tv));
    include_file_crt->signal_edited().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_include_file_crt_edited), this->m_includes_tv));
    base_class_name_crt->signal_edited().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_include_file_crt_edited), this->m_bases_tv));
    include_delimiter_crc->signal_changed().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_cell_renderer_combo_changed), this->m_includes_tv));
    inheritance_type_crc->signal_changed().connect(sigc::bind<Gtk::TreeView*>(sigc::mem_fun(*this, &ClassInfoPage::on_cell_renderer_combo_changed), this->m_bases_tv));
    this->m_includes_tv->get_selection()->signal_changed().connect(sigc::bind<Gtk::TreeSelection*>(sigc::mem_fun(*this, &ClassInfoPage::on_selection_changed), this->m_includes_tv->get_selection().operator ->()));
    this->m_bases_tv->get_selection()->signal_changed().connect(sigc::bind<Gtk::TreeSelection*>(sigc::mem_fun(*this, &ClassInfoPage::on_selection_changed), this->m_bases_tv->get_selection().operator ->()));
    for (auto child : class_info_grid->get_children()) {
        auto entry = dynamic_cast<Gtk::Entry*>(child);

        if (entry != nullptr) {
            entry->signal_changed().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &ClassInfoPage::on_entry_changed), entry));
            entry->signal_icon_release().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &ClassInfoPage::on_entry_icon_release), entry));
        }
    }

    this->validate_page();
}

/**
 * Gets the class name of the new class.
 * @return Class name.
 */
Glib::ustring ClassInfoPage::get_class_name() const
{
    return this->m_class_name_entry->get_text();
}

/**
 * Gets the namespace of the new class.
 * @return Namespace.
 */
Glib::ustring ClassInfoPage::get_namespace() const
{
    return this->m_namespace_entry->get_text();
}

/**
 * Gets the include preprocessor directive lines to be added to the new class
 * header file.
 * @return String with the code lines for the includes.
 */
Glib::ustring ClassInfoPage::get_includes() const
{
    auto model = Glib::RefPtr<Gtk::ListStore>::cast_static(this->m_includes_tv->get_model());
    Glib::ustring include_lines;

    for (auto iter : model->children()) {
        Glib::ustring delimiter_chars,
                      include_filename;

        iter->get_value(2, delimiter_chars);
        iter->get_value(0, include_filename);
        include_lines += Glib::ustring::compose("#include %1%2%3\n", delimiter_chars.substr(0, 1), include_filename, delimiter_chars.substr(1, 1));
    }

    return include_lines.substr(0, include_lines.size() - 1);
}

/**
 * Gets the base classes of the new class.
 * @return String with the base classes part of the class declaration.
 */
Glib::ustring ClassInfoPage::get_base_classes() const
{
    auto model = Glib::RefPtr<Gtk::ListStore>::cast_static(this->m_bases_tv->get_model());
    Glib::ustring base_classes;

    for (auto iter : model->children()) {
        Glib::ustring inheritance_keyword,
                      base_class_name;

        iter->get_value(2, inheritance_keyword);
        iter->get_value(0, base_class_name);
        base_classes += Glib::ustring::compose("%1 %2, ", inheritance_keyword, base_class_name);
    }

    if (!base_classes.empty()) {
        base_classes = " : " + base_classes.erase(base_classes.size() - 2);
    }

    return base_classes;
}

} // ClassCreator
