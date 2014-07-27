/**
 * @file
 * PreferencesDialog class implementation.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#include "preferencesdialog.hpp"
#include "config.h"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/builder.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Loads the default application's settings if the schema hasn't got values to
 * load from.
 */
void PreferencesDialog::load_default_settings()
{
    auto author           = this->m_settings->get_string("author"),
         copyright_holder = this->m_settings->get_string("copyright-holder");

    if (author.empty()) {
        this->m_author_entry->set_text(Glib::get_real_name());
    }

    if (copyright_holder.empty()) {
        this->m_copyright_holder_entry->set_text(this->m_author_entry->get_text());
    }
}

/**
 * Validate the dialog's widgets' values
 * @return @c TRUE if all values are right.
 */
bool PreferencesDialog::validate_controls() const
{
    return !this->m_author_entry->get_text().empty() &&
           !this->m_copyright_holder_entry->get_text().empty() &&
           !this->m_header_extension_entry->get_text().empty() &&
           !this->m_source_extension_entry->get_text().empty();
}

/**
 * Sets the clear icon sensitivity of the Gtk::Entry which emitted the signal
 * and validates the dialog's controls.
 * @param sender The Gtk::Entry object which emitted the changed signal.
 */
void PreferencesDialog::on_entry_changed(Gtk::Entry *sender)
{
    sender->set_icon_sensitive(Gtk::ENTRY_ICON_SECONDARY, !sender->get_text().empty());

    if (sender == this->m_date_format_entry) {
        auto date_str    = Glib::DateTime::create_now_local().format(this->m_date_format_entry->get_text()),
             example_str = Glib::ustring::compose("%1: %2", _("Example"), date_str);

        this->m_date_example_label->set_text(example_str);
    }

    this->get_widget_for_response(Gtk::RESPONSE_ACCEPT)->set_sensitive(this->validate_controls());
}

/**
 * Clears the text of the Gtk::Entry Object emitting the signal when the
 * secondary icon is released.
 * @param icon_pos The position of the released icon.
 * @param event The button release event.
 * @param sender The Gtk::Entry object emitting the icon_release signal.
 */
void PreferencesDialog::on_entry_icon_release(Gtk::EntryIconPosition icon_pos, const GdkEventButton *event, Gtk::Entry *sender)
{
    if (icon_pos == Gtk::ENTRY_ICON_SECONDARY && event->button == GDK_BUTTON_PRIMARY) {
        sender->set_text(Glib::ustring());
    }
}

/**
 * Constructor.
 * @param modal Whether
 */
PreferencesDialog::PreferencesDialog(bool modal) :
    Gtk::Dialog(_("Preferences"), modal)
{
    Gtk::Grid *preferences_grid = nullptr;
    auto gui_res_path = Glib::build_filename(APP_PATH, "resources", "gui", "preferencesgrid.glade");
    auto builder = Gtk::Builder::create_from_resource(gui_res_path);
    auto accept_button = this->add_button("_Accept", Gtk::RESPONSE_ACCEPT),
         cancel_button = this->add_button("_Cancel", Gtk::RESPONSE_CANCEL);

    builder->get_widget("PreferencesGrid",      preferences_grid);
    builder->get_widget("AuthorEntry",          this->m_author_entry);
    builder->get_widget("CopyrightHolderEntry", this->m_copyright_holder_entry);
    builder->get_widget("DateFormatEntry",      this->m_date_format_entry);
    builder->get_widget("HeaderExtensionEntry", this->m_header_extension_entry);
    builder->get_widget("SourceExtensionEntry", this->m_source_extension_entry);
    builder->get_widget("LicenseInfoTV",        this->m_license_info_tv);
    builder->get_widget("DateExampleLabel",     this->m_date_example_label);


    accept_button->set_image_from_icon_name("dialog-ok");
    accept_button->get_image()->set_margin_right(10);

    cancel_button->set_image_from_icon_name("dialog-cancel");
    cancel_button->get_image()->set_margin_right(10);

    this->get_content_area()->add(*preferences_grid);

    // Signals
    for (auto child : preferences_grid->get_children()) {
        auto entry = dynamic_cast<Gtk::Entry*>(child);

        if (entry != nullptr) {
            entry->signal_changed().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &PreferencesDialog::on_entry_changed), entry));
            entry->signal_icon_release().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &PreferencesDialog::on_entry_icon_release), entry));
        }
    }

    this->m_settings = Gio::Settings::create(APP_ID, APP_PATH);
    this->m_settings->delay();
    this->m_settings->bind("author", this->m_author_entry->property_text());
    this->m_settings->bind("copyright-holder", this->m_copyright_holder_entry->property_text());
    this->m_settings->bind("date-format", this->m_date_format_entry->property_text());
    this->m_settings->bind("header-ext", this->m_header_extension_entry->property_text());
    this->m_settings->bind("source-ext", this->m_source_extension_entry->property_text());
    this->m_settings->bind("license-info", this->m_license_info_tv->get_buffer()->property_text());

    this->load_default_settings();
}

/**
 * Applies the changes made to the application's settings.
 */
void PreferencesDialog::apply_settings() const
{
    // Need to set the license info manually because the binding only retrieves
    // the data from the schema in this case.
    this->m_settings->set_string("license-info", this->m_license_info_tv->get_buffer()->get_text());
    this->m_settings->apply();
}

} // ClassCreator
