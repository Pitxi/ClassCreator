/**
 * @file
 * Punto de entrada de la aplicación.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "config.hpp"
#include "classcreatorassistant.hpp"
#include "resourcemanager.hpp"
#include <glibmm/init.h>
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/application.h>

/**
 * Ejemplo de uso de la librería GTKMM.
 * @param argc Número de argumentos en la llamada.
 * @param argv Array con los argumentos.
 */
int main(int argc, char** argv)
{
    std::locale::global(std::locale(""));
    textdomain(PACKAGE);
    bindtextdomain(PACKAGE, LOCALE_DIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
#ifdef DEBUG
    Glib::init();
    Glib::setenv("GSETTINGS_SCHEMA_DIR", "./schemas", true);
#endif // DEBUG
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, APP_ID);
    auto main_window_title = Glib::ustring::compose("%1 v%2.%3.%4", Glib::get_application_name(), VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
    Tools::ResourceManager res_man(APP_PATH);
    ClassCreator::ClassCreatorAssistant main_window(main_window_title);
    auto icon_name = Glib::ustring::compose("%1.png", PACKAGE);
    auto main_icon = res_man.get_image(Glib::build_filename("resources", "icons", "48x48", icon_name));

    Glib::set_application_name(PROJECT_NAME);
    main_window.set_icon(main_icon);

    return app->run(main_window);
}
