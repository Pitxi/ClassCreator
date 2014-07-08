/**
 * @file
 * ClassInfoPage class declaration.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#ifndef CLASSINFOPAGE_HPP
#define CLASSINFOPAGE_HPP

#include "assistantpage.hpp"
#include <gtkmm/toolbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/cellrenderercombo.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Assistant page used to retrieve the new C++ Class data from the user.
 */
class ClassInfoPage final : public AssistantPage
{
private:
    Gtk::ToolButton *m_add_include_tb    = nullptr,
                    *m_remove_include_tb = nullptr,
                    *m_add_base_tb       = nullptr,
                    *m_remove_base_tb    = nullptr;
    Gtk::TreeView *m_includes_tv         = nullptr,
                  *m_bases_tv            = nullptr;
    Gtk::Entry *m_class_name_entry       = nullptr,
               *m_namespace_entry        = nullptr;

    void on_add_tb_clicked(Gtk::TreeView *sender);
    void on_remove_tb_clicked(Gtk::TreeView *sender);
    void on_include_file_crt_edited(const Glib::ustring &path, const Glib::ustring &new_text, Gtk::TreeView *tree_view);
    void on_cell_renderer_combo_changed(const Glib::ustring &path, const Gtk::TreeIter &iter, Gtk::TreeView *tree_view);
    void on_selection_changed(Gtk::TreeSelection *sender);
    void on_entry_changed(Gtk::Entry *sender);
    void on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender);
    bool validate_page();

public:
    ClassInfoPage();

    Glib::ustring get_class_name() const;
    Glib::ustring get_namespace() const;
    Glib::ustring get_includes() const;
    Glib::ustring get_base_classes() const;
};

} // ClassCreator

#endif // CLASSINFOPAGE_HPP
