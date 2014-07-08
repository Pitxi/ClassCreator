/**
 * @file
 * Punto de entrada de la aplicación.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "config.h"
#include "classcreatorassistant.h"
#include <glibmm/miscutils.h>
#include <gtkmm/application.h>
#include <iostream>

/**
 * Ejemplo de uso de la librería GTKMM.
 * @param argc Número de argumentos en la llamada.
 * @param argv Array con los argumentos.
 */
int main(int argc, char** argv)
{
#ifdef DEBUG
    Glib::init();
    Glib::setenv("GSETTINGS_SCHEMA_DIR", "./schemas", true);
#endif // DEBUG
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, APP_ID);
    Glib::set_application_name(PROJECT_NAME);

    ClassCreator::ClassCreatorAssistant main_window(Glib::ustring::compose("%1 v%2.%3.%4", Glib::get_application_name(), VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO));

    return app->run(main_window);
}
