/**
 * @file
 * Implementación de la clase IntroPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "intropage.h"
#include "preferencesdialog.h"
#include <glibmm/i18n.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Spawns the preferences dialog.
 */
void IntroPage::onPreferencesClicked()
{
    PreferencesDialog dialog;

    if (dialog.run() == Gtk::RESPONSE_ACCEPT) {
        dialog.apply();
    }
}

/**
 * Constructor.
 */
IntroPage::IntroPage() :
    AssistantPage(_("Create a new C++ Class"))
{
    auto *label_intro = Gtk::manage(new Gtk::Label(_("With this wizard you can create a new C++ Class for your projects.\n\n"
                                                           "The created class includes default doxygen comments, default constructor and default destructor.\n"
                                                           "You can also specify a namespace and the base classes from which the new class will derive.\n\n"), 0.0, 0.5));
    Gtk::Button *pref_button = Gtk::manage(new Gtk::Button(_("Application's _Preferences"), true));

    label_intro->set_line_wrap_mode(Pango::WRAP_WORD);
    label_intro->set_line_wrap();

    pref_button->set_image_from_icon_name("preferences-system", Gtk::ICON_SIZE_DIALOG);
    pref_button->set_tooltip_text(_("Change application's preferences"));
    pref_button->get_image()->set_margin_right(10);
    pref_button->get_image()->set_margin_bottom(5);
    pref_button->get_image()->set_margin_top(5);
    pref_button->set_halign(Gtk::ALIGN_CENTER);

    this->setHeaderMargin(150);
    this->attach(*label_intro, 0, 0, 1, 1);
    this->attach(*pref_button, 0, 1, 1, 1);

    pref_button->signal_clicked().connect(sigc::mem_fun(*this, &IntroPage::onPreferencesClicked));
}

} // ClassCreator
