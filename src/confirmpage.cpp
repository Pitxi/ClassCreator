/**
 * @file
 * ConfirmPage class implementation.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#include "confirmpage.hpp"
#include "config.hpp"
#include <gtkmm/object.h>
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/builder.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Validates the current assistant page and emits the validated signal.
 * @return @c TRUE if the current page is valid or @c FALSE otherwise.
 */
bool ConfirmPage::validate_page()
{
    bool A = this->m_header_file_cb->get_active(),
         B = !this->m_header_file_entry->get_text().empty(),
         C = this->m_source_file_cb->get_active(),
         D = !this->m_source_file_entry->get_text().empty(),
         is_valid = (!A && C && D) || (A && B && !C) || (A && B && D);

    this->m_signal_validated.emit(is_valid);

    return is_valid;
}

/**
 * Removes the comments from the given template.
 * @param template_contents String with the template contents.
 * @return The given template contents without comments.
 */
Glib::ustring ConfirmPage::remove_comments(const Glib::ustring &template_contents)
{
    auto remove_comments_regex = Glib::Regex::create("((?:\\/\\*\\*).*?(?:\\*\\/))[\\n\\r]*", Glib::REGEX_DOTALL);

    return remove_comments_regex->replace(template_contents, 0, Glib::ustring(), Glib::REGEX_MATCH_NEWLINE_ANY);
}

/**
 * Removes the namespace section from the given template.
 * @param template_contents String with the template contents.
 * @return The given template contents without the namespace section.
 */
Glib::ustring ConfirmPage::remove_namespace(const Glib::ustring &template_contents)
{
    auto remove_namespace_regex = Glib::Regex::create("(?:\\/\\*+\\s+\\**\\s*%NAMESPACE%\\s+namespace.\\s*\\**\\/\\s+namespace\\s+%NAMESPACE%\\s+{\\s+)|(?:\\s*}\\s*\\/\\/\\s*%NAMESPACE%)");

    return remove_namespace_regex->replace(template_contents, 0, Glib::ustring(), Glib::REGEX_MATCH_NEWLINE_ANY);
}

/**
 * Sets the default file name based on classname and the application's settings.
 * @param class_name The class name.
 * @param file_entry The file path entry.
 */
void ConfirmPage::set_default_filename(const Glib::ustring &class_name, Gtk::Entry *file_entry)
{
    auto last_folder = this->m_settings->get_string("last-folder"),
         ext = (file_entry == this->m_header_file_entry) ? this->m_settings->get_string("header-ext") : this->m_settings->get_string("source-ext"),
         header_file_base_name = Glib::ustring::compose("%1.%2", class_name.lowercase(), ext);

    if (last_folder.empty()) {
        last_folder = Glib::get_home_dir();
    }

    file_entry->set_text(Glib::build_filename(last_folder, header_file_base_name));
}

/**
 * Sets sender's clear text icon sensitivity accordingly to the contents
 * of the entry and validates the page.
 * @param sender The entry that emitted the signal.
 */
void ConfirmPage::on_entry_changed(Gtk::Entry *sender)
{
    sender->set_icon_sensitive(Gtk::ENTRY_ICON_SECONDARY, !sender->get_text().empty());

    this->validate_page();
}

/**
 * Clears sender's text when it's clear text icon is released.
 * @param icon_pos The position of the released icon.
 * @param event The button release event.
 * @param sender The entry that emitted the signal.
 */
void ConfirmPage::on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender)
{
    if (icon_pos == Gtk::ENTRY_ICON_SECONDARY && event->button == GDK_BUTTON_PRIMARY) {
        sender->set_text(Glib::ustring());
    }
}

/**
 * Sets the sensitivity of the related controls when the sender is toggled.
 * @param sender The check button that emitted the signal.
 */
void ConfirmPage::on_file_cb_toggled(Gtk::CheckButton *sender)
{
    auto entry  = this->m_header_file_entry;
    auto button = this->m_header_file_save_as_button;
    auto view   = this->m_header_sv;

    if (sender == this->m_source_file_cb) {
        entry  = this->m_source_file_entry;
        button = this->m_source_file_save_as_button;
        view   = this->m_source_sv;
    }

    entry->set_sensitive(sender->get_active());
    button->set_sensitive(sender->get_active());
    view->set_sensitive(sender->get_active());
    view->get_source_buffer()->set_highlight_syntax(sender->get_active());

    this->validate_page();
}

/**
 * Shows a dialog to choose the name and location of the related file.
 * @param sender The button which emitted the signal.
 */
void ConfirmPage::on_file_button_clicked(Gtk::Button *sender)
{
    auto entry  = this->m_header_file_entry;
    Glib::ustring title(_("Choose a name and location for the header file..."));
    auto header_filter = Gtk::FileFilter::create(),
         source_filter = Gtk::FileFilter::create(),
         all_filter  = Gtk::FileFilter::create();

    header_filter->set_name(_("C++ header files"));
    header_filter->add_mime_type("text/x-c++hdr");

    source_filter->set_name(_("C++ source files"));
    source_filter->add_mime_type("text/x-c++src");

    all_filter->set_name(_("All files"));
    all_filter->add_pattern("*");

    if (sender == this->m_source_file_save_as_button) {
        entry = this->m_source_file_entry;
        title = _("Choose a name and location for the source file...");
    }

    Gtk::FileChooserDialog dialog(title, Gtk::FILE_CHOOSER_ACTION_SAVE);

    if (sender == this->m_header_file_save_as_button) {
        dialog.add_filter(header_filter);
    } else {
        dialog.add_filter(source_filter);
    }

    dialog.add_filter(all_filter);
    dialog.set_filename(entry->get_text());
    dialog.set_current_name(Glib::filename_display_basename(entry->get_text()));

    if (entry->get_text().empty()) {
        dialog.set_current_folder(this->m_settings->get_string("last-folder"));
    }

    dialog.add_button(_("Select"), Gtk::RESPONSE_ACCEPT)->set_image_from_icon_name("document-save-as");
    dialog.add_button(_("Cancel"), Gtk::RESPONSE_CANCEL)->set_image_from_icon_name("gtk-cancel");

    if (dialog.run() == Gtk::RESPONSE_ACCEPT) {
        entry->set_text(dialog.get_filename());
        this->m_settings->set_string("last-folder", dialog.get_current_folder());
    }
}

/**
 * Constructor.
 */
ConfirmPage::ConfirmPage() :
    AssistantPage(_("Confirm the new C++ Class files creation")),
    m_resource_manager(Glib::build_filename(APP_PATH, "resources")),
    m_settings(Gio::Settings::create(APP_ID, APP_PATH))
{
    Gsv::init();
    Gtk::Grid *confirm_grid = nullptr;
    auto gui_res_path = Glib::build_filename(APP_PATH, "resources", "gui", "confirmgrid.glade");
    auto builder = Gtk::Builder::create_from_resource(gui_res_path);
    auto language_manager = Gsv::LanguageManager::create();
    auto sv_language = language_manager->get_language("cpp");
    auto css_provider = Gtk::CssProvider::create();

    builder->get_widget("ConfirmGrid",            confirm_grid);
    builder->get_widget("HeaderFrame",            this->m_header_frame);
    builder->get_widget("SourceFrame",            this->m_source_frame);
    builder->get_widget("HeaderSV",               this->m_header_sv);
    builder->get_widget("SourceSV",               this->m_source_sv);
    builder->get_widget("HeaderFileCB",           this->m_header_file_cb);
    builder->get_widget("SourceFileCB",           this->m_source_file_cb);
    builder->get_widget("HeaderFileEntry",        this->m_header_file_entry);
    builder->get_widget("SourceFileEntry",        this->m_source_file_entry);
    builder->get_widget("HeaderFileSaveAsButton", this->m_header_file_save_as_button);
    builder->get_widget("SourceFileSaveAsButton", this->m_source_file_save_as_button);

    css_provider->load_from_data("GtkSourceView {font: monospace 10;}");
    this->m_header_sv->get_style_context()->add_provider(css_provider, 0);
    this->m_header_sv->set_source_buffer(Gsv::Buffer::create(sv_language));
    this->m_header_sv->get_source_buffer()->set_highlight_syntax();
    this->m_header_sv->get_source_buffer()->set_highlight_matching_brackets();

    this->m_source_sv->get_style_context()->add_provider(css_provider, 0);
    this->m_source_sv->set_source_buffer(Gsv::Buffer::create(sv_language));
    this->m_source_sv->get_source_buffer()->set_highlight_syntax();
    this->m_source_sv->get_source_buffer()->set_highlight_matching_brackets();

    this->get_content_area()->attach(*confirm_grid, 0, 0, 1, 1);

    // Signals
    for (auto child : confirm_grid->get_children()) {
        auto entry        = dynamic_cast<Gtk::Entry*>(child);
        auto check_button = dynamic_cast<Gtk::CheckButton*>(child);
        auto button       = dynamic_cast<Gtk::Button*>(child);

        if (entry != nullptr) {
            entry->signal_changed().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &ConfirmPage::on_entry_changed), entry));
            entry->signal_icon_release().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &ConfirmPage::on_entry_icon_release), entry));
        } else if (check_button != nullptr) {
            check_button->signal_toggled().connect(sigc::bind<Gtk::CheckButton*>(sigc::mem_fun(*this, &ConfirmPage::on_file_cb_toggled), check_button));
        } else if (button != nullptr) {
            button->signal_clicked().connect(sigc::bind<Gtk::Button*>(sigc::mem_fun(*this, &ConfirmPage::on_file_button_clicked), button));
        }
    }
}

/**
 * Generates the header file contents and put them on the header file source
 * view.
 * @param author Author of the class.
 * @param copyright_holder Copyright holder of the class.
 * @param date_format Format of the date for the date field.
 * @param license_info License info of the class.
 * @param nspace Namespace.
 * @param class_name Class name.
 * @param includes Include files lines.
 * @param base_classes Base classes part of the class declaration.
 * @param include_doc @c TRUE to include documentation on the file.
 */
void ConfirmPage::generate_header(const Glib::ustring &author,
                                  const Glib::ustring &copyright_holder,
                                  const Glib::ustring &date_format,
                                  const Glib::ustring &license_info,
                                  const Glib::ustring &nspace,
                                  const Glib::ustring &class_name,
                                  const Glib::ustring &includes,
                                  const Glib::ustring &base_classes,
                                  bool include_doc)
{
    auto header_template = this->m_resource_manager.get_text("templates/header.template"),
         class_decl   = Glib::ustring::compose("class %1%2", class_name, base_classes);
    auto s_buffer = this->m_header_sv->get_source_buffer();
    auto date = Glib::DateTime::create_now_local();

    if (this->m_header_file_entry->get_text().empty()) {
        this->set_default_filename(class_name, this->m_header_file_entry);
    }

    auto header_basename = Glib::filename_display_basename(this->m_header_file_entry->get_text());
    auto include_guard = Glib::Regex::create("[\\s.]")->replace(header_basename.uppercase(), 0, "_", Glib::REGEX_MATCH_NEWLINE_ANY);

    if (nspace.empty()) {
        header_template = this->remove_namespace(header_template);
    }

    if (includes.empty()) {
        header_template = Glib::Regex::create("%INCLUDES%[\\s]*")->replace(header_template, 0, Glib::ustring(), Glib::REGEX_MATCH_NEWLINE_ANY);
    }

    if (!include_doc) {
        header_template = this->remove_comments(header_template);
    }

    Glib::ustring tokens[10][2] {{"%AUTHOR%", author},
                                 {"%COPYRIGHT_HOLDER%", copyright_holder},
                                 {"%DATE%", date.format(date_format)},
                                 {"%YEAR%", date.format("%Y")},
                                 {"%LICENSE_INFO%", license_info},
                                 {"%INCLUDE_GUARD%", include_guard},
                                 {"%NAMESPACE%",  nspace},
                                 {"%CLASS_NAME%", class_name},
                                 {"%CLASS_DECL%", class_decl},
                                 {"%INCLUDES%",   includes}};

    for (auto token : tokens) {
        header_template = Glib::Regex::create(token[0])->replace(header_template, 0, token[1], Glib::REGEX_MATCH_NEWLINE_ANY);
    }

    s_buffer->set_text(header_template);
    s_buffer->select_range(s_buffer->begin(), s_buffer->begin());
}

/**
 * Generates the source file contents and put them on the source file source
 * view.
 * @param author Author of the class.
 * @param copyright_holder Copyright holder of the class.
 * @param date_format Format of the date for the date field.
 * @param license_info License info of the class.
 * @param nspace Namespace.
 * @param class_name Class name.
 * @param include_doc @c TRUE to include documentation on the file.
 */
void ConfirmPage::generate_source(const Glib::ustring &author,
                                  const Glib::ustring &copyright_holder,
                                  const Glib::ustring &date_format,
                                  const Glib::ustring &license_info,
                                  const Glib::ustring &nspace,
                                  const Glib::ustring &class_name,
                                  bool include_doc)
{
    auto source_template = this->m_resource_manager.get_text("templates/source.template");
    auto s_buffer = this->m_source_sv->get_source_buffer();
    auto header_file = Glib::filename_display_basename(this->m_header_file_entry->get_text());
    auto date = Glib::DateTime::create_now_local();

    if (this->m_source_file_entry->get_text().empty()) {
        this->set_default_filename(class_name, this->m_source_file_entry);
    }

    if (this->m_header_file_entry->get_text().empty()) {
        this->set_default_filename(class_name, this->m_header_file_entry);
    }

    if (nspace.empty()) {
        source_template = this->remove_namespace(source_template);
    }

    if (!include_doc) {
        source_template = this->remove_comments(source_template);
    }

    Glib::ustring tokens[8][2] {{"%AUTHOR%", author},
                                {"%COPYRIGHT_HOLDER%", copyright_holder},
                                {"%DATE%", date.format(date_format)},
                                {"%YEAR%", date.format("%Y")},
                                {"%LICENSE_INFO%", license_info},
                                {"%HEADER_FILE%", header_file},
                                {"%NAMESPACE%",  nspace},
                                {"%CLASS_NAME%", class_name}};

    for (auto token : tokens) {
        source_template = Glib::Regex::create(token[0])->replace(source_template, 0, token[1], Glib::REGEX_MATCH_NEWLINE_ANY);
    }

    s_buffer->set_text(source_template);
    s_buffer->select_range(s_buffer->begin(), s_buffer->begin());
}

/**
 * Saves the contents of the header and source files.
 */
void ConfirmPage::save() const
{
    if (this->m_header_file_cb->get_active()) {
        auto header_stream = Gio::File::create_for_path(this->m_header_file_entry->get_text())->replace(std::string(), true);

        header_stream->write(this->m_header_sv->get_source_buffer()->get_text(false));
        header_stream->close();
    }

    if (this->m_source_file_cb->get_active()) {
        auto source_stream = Gio::File::create_for_path(this->m_source_file_entry->get_text())->replace(std::string(), true);

        source_stream->write(this->m_source_sv->get_source_buffer()->get_text(false));
        source_stream->close();
    }
}

} // ClassCreator
