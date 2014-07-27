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
#include "config.h"
#include "resourcemanager.hpp"
#include "preferencesdialog.hpp"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <giomm/settings.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/aboutdialog.h>

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
 * Shows the about dialog.
 */
void IntroPage::on_about_button_clicked()
{
    Gtk::AboutDialog dialog;
    Tools::ResourceManager manager(APP_PATH);
    auto settings = Gio::Settings::create(APP_ID, APP_PATH);
    auto icon_filename = Glib::build_filename("resources", "icons", "256x256", Glib::ustring::compose("%1.png", PACKAGE));

    dialog.set_position(Gtk::WIN_POS_CENTER);
    dialog.set_logo(manager.get_image(icon_filename));
    dialog.set_comments(Glib::ustring::compose(_("%1 is a small tool conceived as\na wizard to aid in creating new clases for C++ projects."), PROJECT_NAME));
    dialog.set_translator_credits(_("Javier Campón Pichardo (english and spanish)"));
    dialog.set_copyright("Copyright 2014 Javier Campón Pichardo");
    dialog.set_license_type(Gtk::LICENSE_GPL_3_0);
    dialog.set_authors({"Javier Campón Pichardo"});
    dialog.set_website_label(_("GitHub project site"));
    dialog.set_website("https://github.com/Pitxi/ClassCreator");

    dialog.run();
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

    this->m_header_label->set_margin_bottom(200);

    intro_label->set_margin_bottom(100);
    intro_label->set_halign(Gtk::ALIGN_CENTER);
    intro_label->set_hexpand(true);

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
    about_button->signal_clicked().connect(sigc::mem_fun(*this, &IntroPage::on_about_button_clicked));
}

} // ClassCreator
