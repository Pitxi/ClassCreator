/**
 * @file
 * IntroPage class implementation.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#include "intropage.hpp"
#include "config.hpp"
#include "resourcemanager.hpp"
#include "preferencesdialog.hpp"
#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Show the Preferences dialog.
 */
void IntroPage::on_preferences_button_clicked()
{
    PreferencesDialog dialog(true);

    if (dialog.run() == Gtk::RESPONSE_ACCEPT) {
        dialog.apply_settings();
    }
}

/**
 * Validates the current page.
 * This is a dummy method. It always returns true because the intro page is
 * always completed.
 * @return Always @c TRUE.
 */
bool IntroPage::validate_page()
{
    return true;
}

/**
 * Constructor.
 */
IntroPage::IntroPage() : AssistantPage(_("Create a new C++ class"))
{
    auto intro_label = Gtk::manage(new Gtk::Label(Glib::ustring::compose(_("The %1 tool will assist you in the creation of a new C++ class\n"
                                                                           "by generating the needed source and header files with the provided information.\n\n"
                                                                           "In this page you also can set the %1 preferences by clicking on the preferences button.\n\n"
                                                                           "Press Continue to start or Cancel to exit."), PROJECT_NAME)));
    auto preferences_button = Gtk::manage(new Gtk::Button(_("_Preferences"), true)),
         about_button = Gtk::manage(new Gtk::Button(Glib::ustring::compose(_("_About %1..."), PROJECT_NAME), true));

    this->m_header_label->set_margin_bottom(100);

    intro_label->set_margin_bottom(100);

    preferences_button->set_image_from_icon_name("preferences-system", Gtk::ICON_SIZE_DIALOG);
    preferences_button->get_image()->set_margin_right(10);
    preferences_button->set_relief(Gtk::RELIEF_NONE);
    preferences_button->set_hexpand(false);
    preferences_button->set_halign(Gtk::ALIGN_END);

    about_button->set_image_from_icon_name("help-about", Gtk::ICON_SIZE_DIALOG);
    about_button->get_image()->set_margin_right(10);
    about_button->set_relief(Gtk::RELIEF_NONE);
    about_button->set_hexpand(false);
    about_button->set_halign(Gtk::ALIGN_START);

    this->get_content_area()->attach(*intro_label, 0, 0, 2, 1);
    this->get_content_area()->attach(*preferences_button, 0, 1, 1, 1);
    this->get_content_area()->attach(*about_button, 1, 1, 1, 1);

    this->show_all();

    // Signals
    preferences_button->signal_clicked().connect(sigc::mem_fun(*this, &IntroPage::on_preferences_button_clicked));
}

} // ClassCreator
