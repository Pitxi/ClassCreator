/**
 * @file
 * Implementación de la clase ClassCreatorAssistant.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "classcreatorassistant.h"
#include "config.h"
#include "resources.h"
#include <glibmm/i18n.h>
#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <glibmm/regex.h>
#include <giomm/resource.h>
#include <gtkmm/cssprovider.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Parsea la plantilla especificada.
 * @param template_str Cadena con el contenido de la plantilla que será
 * parseada.
 * @return Resultado de parsear la plantilla.
 */
Glib::ustring ClassCreatorAssistant::parse(Glib::ustring template_str) const
{
    auto regex = Glib::Regex::create("[\\.\\s]");
    Glib::ustring header_filename = this->m_class_page->getHeaderFilename(),
                  header_basename = g_filename_display_basename(header_filename.c_str()),
                  header_guard = regex->replace(header_basename, 0, "_", Glib::REGEX_MATCH_NOTEMPTY).uppercase(),
                  tokens[8][2]{{"AUTHOR", this->m_documentation_page->getAuthor()},
                               {"DATE", this->m_documentation_page->getDateStr()},
                               {"LICENSE", this->m_documentation_page->getLicense()},
                               {"HEADER_GUARD", header_guard},
                               {"NAMESPACE", this->m_class_page->getNamespace()},
                               {"CLASSNAME", this->m_class_page->getClassName()},
                               {"BASE_SYNTAX", this->m_class_page->getBaseClassesSyntax()},
                               {"HEADER_FILENAME", header_basename}};

    for (auto &token : tokens) {
        auto regex = Glib::Regex::create(Glib::ustring::compose("@@%1@@", token[0]));
        Glib::ustring replacement = token[1];
        template_str = regex->replace(template_str, 0, replacement, Glib::REGEX_MATCH_NOTEMPTY);
    }

    return template_str;
}

/**
 * Sobrecarga del método Gtk::Window::on_hide.
 */
void ClassCreatorAssistant::on_hide()
{
    Gtk::Assistant::on_hide();
    Gtk::Assistant::destroy_();
}

/**
 * Si la nueva página actual establecida es la página de confirmación
 * de cambios se parsean las plantillas.
 * @param page Nueva página actual del asistente.
 */
void ClassCreatorAssistant::onPrepare(Gtk::Widget *page)
{
    if (page == this->m_confirm_page) {
        Glib::ustring template_header_resource_name = Glib::build_filename(APP_PATH, "templates", "class"),
                      template_source_resource_name = template_header_resource_name;
        gchar *template_header, *template_source;


        if (!this->m_class_page->getNamespace().empty()) {
            template_header_resource_name += ".namespace";
            template_source_resource_name += ".namespace";
        }

        if (this->m_documentation_page->getIncludeDocumentation()) {
            template_header_resource_name += ".doc";
            template_source_resource_name += ".doc";
        }

        template_header_resource_name += ".header.in";
        template_source_resource_name += ".source.in";

        auto header_bytes = g_resources_lookup_data(template_header_resource_name.c_str(), G_RESOURCE_LOOKUP_FLAGS_NONE, nullptr),
             source_bytes = g_resources_lookup_data(template_source_resource_name.c_str(), G_RESOURCE_LOOKUP_FLAGS_NONE, nullptr);
        auto header_size = g_bytes_get_size(header_bytes),
             source_size = g_bytes_get_size(source_bytes);

        template_header = (gchar*)g_bytes_get_data(header_bytes, &header_size);
        template_source = (gchar*)g_bytes_get_data(source_bytes, &source_size);

        this->m_confirm_page->setHeaderContent(this->parse(template_header));
        this->m_confirm_page->setSourceContent(this->parse(template_source));
    }
}

/**
 * Establece el estado de completado de la página.
 * @param completed @c TRUE para cambiar el estado de la página a completada o
 * @c FALSE para no completada.
 * @param page Página que enví la señal.
 */
void ClassCreatorAssistant::onPageCompletionStatusChanged(bool completed, Gtk::Widget *page)
{
    this->set_page_complete(*page, completed);
}

/**
 * Crea los ficheros de la nueva clase con su contenido al confirmar.
 */
void ClassCreatorAssistant::onApply()
{
    Glib::file_set_contents(this->m_class_page->getHeaderFilename(), this->m_confirm_page->getHeaderContent());
    Glib::file_set_contents(this->m_class_page->getSourceFilename(), this->m_confirm_page->getSourceContent());
}

    /**
     * Constructor.
     * @param title Título de la ventana.
     */
    ClassCreatorAssistant::ClassCreatorAssistant(const Glib::ustring &title)
    {
        this->m_intro_page = Gtk::manage(new IntroPage);
        this->m_class_page = Gtk::manage(new ClassPage);
        this->m_documentation_page = Gtk::manage(new DocumentationPage);
        this->m_confirm_page = Gtk::manage(new ConfirmPage);
        auto css_provider = Gtk::CssProvider::create();

        Glib::ustring css =
                "GtkLabel.header_label {"
                "    background-image: -gtk-gradient (linear, left top, right top, from (rgba (128, 180, 0, 0.75)), color-stop (0.25, rgba (128, 180, 0, 0.75)), to (rgba (128, 180, 0, 0)));"
                "    font-size: 50pt;"
                "    border-radius: 5px;"
                "    padding: 5px;"
                "}";

        css_provider->load_from_data(css);

        this->get_style_context()->add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        this->append_page(*this->m_intro_page);
        this->set_page_title(*this->m_intro_page, _("Introduction"));
        this->set_page_type(*this->m_intro_page, Gtk::ASSISTANT_PAGE_INTRO);
        this->set_page_complete(*this->m_intro_page);

        this->append_page(*this->m_class_page);
        this->set_page_title(*this->m_class_page, _("Class info"));
        this->set_page_type(*this->m_class_page, Gtk::ASSISTANT_PAGE_CONTENT);

        this->append_page(*this->m_documentation_page);
        this->set_page_title(*this->m_documentation_page, _("Documentation"));
        this->set_page_type(*this->m_documentation_page, Gtk::ASSISTANT_PAGE_CONTENT);

        this->append_page(*this->m_confirm_page);
        this->set_page_title(*this->m_confirm_page, _("Preview"));
        this->set_page_type(*this->m_confirm_page, Gtk::ASSISTANT_PAGE_CONFIRM);
        this->set_page_complete(*this->m_confirm_page);

        this->set_position(Gtk::WIN_POS_CENTER);
        this->set_title(title);
        this->show_all();

        for (int index = 0; index < this->get_n_pages(); ++index) {
            AssistantPage *page = dynamic_cast<AssistantPage *>(this->get_nth_page(index));
            page->signal_page_completion_status_changed().connect(sigc::bind(sigc::mem_fun(*this, &ClassCreatorAssistant::onPageCompletionStatusChanged), page));
        }

        this->signal_prepare().connect(sigc::mem_fun(*this, &ClassCreatorAssistant::onPrepare));
        this->signal_cancel().connect(sigc::mem_fun(*this, &ClassCreatorAssistant::on_hide));
        this->signal_close().connect(sigc::mem_fun(*this, &ClassCreatorAssistant::on_hide));
        this->signal_apply().connect(sigc::mem_fun(*this, &ClassCreatorAssistant::onApply));

        auto icon_bytes = g_resources_lookup_data("/org/kazires/classcreator/icons/main_icon.png", G_RESOURCE_LOOKUP_FLAGS_NONE, nullptr);
        auto icon_size = g_bytes_get_size(icon_bytes);
        auto icon_data = g_bytes_get_data(icon_bytes, &icon_size);

        this->set_icon(Gdk::Pixbuf::create_from_inline(icon_size, (const guint8*)icon_data));

        this->m_documentation_page->setIncludeDocumentation(true);
    }
} // CPPTest
