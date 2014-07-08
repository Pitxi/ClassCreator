/**
 * @file
 * ConfirmPage class declaration.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#ifndef CONFIRMPAGE_HPP
#define CONFIRMPAGE_HPP

#include "assistantpage.hpp"
#include "resourcemanager.hpp"
#include <giomm/settings.h>
#include <gtkmm/frame.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/entry.h>
#include <gtksourceviewmm.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Confirmation page.
 */
class ConfirmPage final : public AssistantPage
{
private:
    Gtk::Frame *m_header_frame                = nullptr,
               *m_source_frame                = nullptr;
    Gsv::View *m_header_sv                    = nullptr,
              *m_source_sv                    = nullptr;
    Gtk::CheckButton *m_header_file_cb        = nullptr,
                     *m_source_file_cb        = nullptr;
    Gtk::Entry *m_header_file_entry           = nullptr,
               *m_source_file_entry           = nullptr;
    Gtk::Button *m_header_file_save_as_button = nullptr,
                *m_source_file_save_as_button = nullptr;

    Tools::ResourceManager m_resource_manager;
    Glib::RefPtr<Gio::Settings> m_settings;

    bool validate_page();
    Glib::ustring remove_comments(const Glib::ustring &template_contents);
    Glib::ustring remove_namespace(const Glib::ustring &template_contents);
    void set_default_filename(const Glib::ustring &class_name, Gtk::Entry *file_entry);
    void on_entry_changed(Gtk::Entry *sender);
    void on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender);
    void on_file_cb_toggled(Gtk::CheckButton *sender);
    void on_file_button_clicked(Gtk::Button *sender);

public:
    ConfirmPage();

    void generate_header(const Glib::ustring &author, const Glib::ustring &copyright_holder, const Glib::ustring &date_format, const Glib::ustring &license_info, const Glib::ustring &nspace, const Glib::ustring &class_name, const Glib::ustring &includes, const Glib::ustring &base_classes, bool include_doc);
    void generate_source(const Glib::ustring &author, const Glib::ustring &copyright_holder, const Glib::ustring &date_format, const Glib::ustring &license_info, const Glib::ustring &nspace, const Glib::ustring &class_name, bool include_doc);
    void save() const;
};

} // ClassCreator

#endif // CONFIRMPAGE_HPP
