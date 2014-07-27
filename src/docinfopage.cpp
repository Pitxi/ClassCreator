/**
 * @file
 * DocInfoPage class implementation.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#include "docinfopage.hpp"
#include "config.h"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <glibmm/regex.h>
#include <gtkmm/builder.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Validates the current assistant page and emits the validated signal.
 * @return @c TRUE if the page is valid or @c FALSE otherwise.
 */
bool DocInfoPage::validate_page()
{
    bool is_valid = !this->m_include_doc_cb->get_active() ||
                    (this->m_include_doc_cb->get_active() &&
                     !this->m_author_entry->get_text().empty() &&
                     !this->m_date_format_entry->get_text().empty());

    this->m_signal_validated.emit(is_valid);

    return is_valid;
}

/**
 * Loads the default values from settings.
 */
void DocInfoPage::load_default_values()
{
    this->m_author_entry->set_text(this->m_settings->get_string("author"));
    this->m_copyright_holder_entry->set_text(this->m_settings->get_string("copyright-holder"));
    this->m_date_format_entry->set_text(this->m_settings->get_string("date-format"));
    this->m_license_info_tv->get_buffer()->set_text(this->m_settings->get_string("license-info"));

    this->validate_page();
}

/**
 * Sets sender's clear text icon sensitivity accordingly to the contents
 * of the entry and validates the page.
 * @param sender The entry that emitted the signal.
 */
void DocInfoPage::on_entry_changed(Gtk::Entry *sender)
{
    sender->set_icon_sensitive(Gtk::ENTRY_ICON_SECONDARY, !sender->get_text().empty());

    if (sender == this->m_date_format_entry) {
        auto date_str    = Glib::DateTime::create_now_local().format(this->m_date_format_entry->get_text()),
             example_str = Glib::ustring::compose("%1: %2", _("Example"), date_str);

        this->m_date_example_label->set_text(example_str);
    }

    this->validate_page();
}

/**
 * Clears sender's text when it's clear text icon is released.
 * @param icon_pos The position of the released icon.
 * @param event The button release event.
 * @param sender The entry that emitted the signal.
 */
void DocInfoPage::on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender)
{
    if (icon_pos == Gtk::ENTRY_ICON_SECONDARY && event->button == GDK_BUTTON_PRIMARY) {
        sender->set_text(Glib::ustring());
    }
}

/**
 * Sets the default value of the related control.
 * @param sender The button that emitted the signal.
 */
void DocInfoPage::on_default_button_clicked(Gtk::Button *sender)
{
    if (sender == this->m_default_author_button) {
        this->m_author_entry->set_text(this->m_settings->get_string("author"));
    } else if (sender == this->m_default_copyright_holder_button) {
        this->m_copyright_holder_entry->set_text(this->m_settings->get_string("copyright-holder"));
    } else if (sender == this->m_default_date_format_button) {
        this->m_date_format_entry->set_text(this->m_settings->get_string("date-format"));
    } else if (this->m_default_license_info_button) {
        this->m_license_info_tv->get_buffer()->set_text(this->m_settings->get_string("license-info"));
    }
}

/**
 * Constructor.
 */
DocInfoPage::DocInfoPage() : AssistantPage(_("Class documentation info"))
{
    Gtk::Grid *doc_info_grid = nullptr;
    auto gui_res_path = Glib::build_filename(APP_PATH, "resources", "gui", "docinfogrid.glade");
    auto builder = Gtk::Builder::create_from_resource(gui_res_path);
    this->m_settings = Gio::Settings::create(APP_ID, APP_PATH);

    builder->get_widget("DocInfoGrid",                  doc_info_grid);
    builder->get_widget("IncludeDocCB",                 this->m_include_doc_cb);
    builder->get_widget("AuthorEntry",                  this->m_author_entry);
    builder->get_widget("CopyrightHolderEntry",         this->m_copyright_holder_entry);
    builder->get_widget("DateFormatEntry",              this->m_date_format_entry);
    builder->get_widget("LicenseInfoTV",                this->m_license_info_tv);
    builder->get_widget("DefaultAuthorButton",          this->m_default_author_button);
    builder->get_widget("DefaultCopyrightHolderButton", this->m_default_copyright_holder_button);
    builder->get_widget("DefaultDateFormatButton",      this->m_default_date_format_button);
    builder->get_widget("DefaultLicenseInfoButton",     this->m_default_license_info_button);
    builder->get_widget("DateExampleLabel",             this->m_date_example_label);

    this->m_header_label->set_margin_bottom(50);

    this->get_content_area()->attach(*doc_info_grid, 0, 0, 1, 1);

    // Signals
    for (auto child : doc_info_grid->get_children()) {
        auto entry  = dynamic_cast<Gtk::Entry*>(child);
        auto button = dynamic_cast<Gtk::Button*>(child);

        if (entry != nullptr) {
            entry->signal_changed().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &DocInfoPage::on_entry_changed), entry));
            entry->signal_icon_release().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &DocInfoPage::on_entry_icon_release), entry));
        } else if(button != nullptr) {
            button->signal_clicked().connect(sigc::bind<Gtk::Button*>(sigc::mem_fun(*this, &DocInfoPage::on_default_button_clicked), button));
        }
    }

    load_default_values();
}

/**
 * Indicates if the documentation will be included in the header and source
 * files.
 * @return @c TRUE if the documentation has to be included or @c false
 * otherwise.
 */
bool DocInfoPage::include_documentation() const
{
    return this->m_include_doc_cb->get_active();
}

/**
 * Gets the author name.
 * @return Author of the class.
 */
Glib::ustring DocInfoPage::get_author() const
{
    return this->m_author_entry->get_text();
}

/**
 * Gets the copyright holder name.
 * @return Copyright holder of the class.
 */
Glib::ustring DocInfoPage::get_copyright_holder() const
{
    return this->m_copyright_holder_entry->get_text();
}

/**
 * Gets the date format as is to be shown on the documentation.
 * @return Date format.
 */
Glib::ustring DocInfoPage::get_date_format() const
{
    return this->m_date_format_entry->get_text();
}

/**
 * Gets the license info as is to be shown in the documentation.
 * @return License info.
 */
Glib::ustring DocInfoPage::get_license_info() const
{
    auto license_info = this->m_license_info_tv->get_buffer()->get_text();

    license_info = Glib::Regex::create("\n")->replace(license_info, 0, "\n * ", Glib::REGEX_MATCH_NEWLINE_ANY);
    license_info = Glib::Regex::create("^\\s\\*\\s*$", Glib::REGEX_MULTILINE)->replace(license_info, 0, " *", Glib::REGEX_MATCH_NEWLINE_ANY);

    return license_info;
}

} // ClassCreator
