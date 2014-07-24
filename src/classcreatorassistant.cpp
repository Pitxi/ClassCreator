/**
 * @file
 * ClassCreatorAssistant class implementation.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#include "classcreatorassistant.hpp"
#include "config.hpp"
#include <glibmm/i18n.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Sets the completeness of the page emitting the signal.
 * @param valid @c TRUE if the page is valid or @c FALSE otherwise.
 * @param sender The assistant page emitting the signal.
 */
void ClassCreatorAssistant::on_page_validated(bool valid, AssistantPage *sender)
{
    this->set_page_complete(*sender, valid);
}

/**
 * Makes preparations for the new current page before showing it when needed.
 * @param page The new current page.
 */
void ClassCreatorAssistant::on_prepare(Gtk::Widget *page)
{
    if (page == this->m_confirm_page) {
        auto author           = this->m_doc_info_page->get_author(),
             copyright_holder = this->m_doc_info_page->get_copyright_holder(),
             date_format      = this->m_doc_info_page->get_date_format(),
             license_info     = this->m_doc_info_page->get_license_info(),
             nspace           = this->m_class_info_page->get_namespace(),
             class_name       = this->m_class_info_page->get_class_name(),
             includes         = this->m_class_info_page->get_includes(),
             base_classes     = this->m_class_info_page->get_base_classes();

        this->m_confirm_page->generate_header(author, copyright_holder, date_format, license_info, nspace, class_name, includes, base_classes, this->m_doc_info_page->include_documentation());
        this->m_confirm_page->generate_source(author, copyright_holder, date_format, license_info, nspace, class_name, this->m_doc_info_page->include_documentation());
    }

    Gtk::Assistant::on_prepare(page);
}

/**
 * Crea los ficheros de la clase en el destino indicado.
 */
void ClassCreatorAssistant::on_apply()
{
    this->m_confirm_page->save();
    this->set_page_complete(*this->m_confirm_page);

    Gtk::Assistant::on_apply();
}

/**
 * Constructor.
 * @param title Window title.
 */
ClassCreatorAssistant::ClassCreatorAssistant(const Glib::ustring &title)
{
    this->m_settings = Gio::Settings::create(APP_ID, APP_PATH);
    this->m_intro_page      = Gtk::manage(new IntroPage);
    this->m_class_info_page = Gtk::manage(new ClassInfoPage);
    this->m_doc_info_page   = Gtk::manage(new DocInfoPage);
    this->m_confirm_page    = Gtk::manage(new ConfirmPage);

    this->append_page(*this->m_intro_page);
    this->set_page_type(*this->m_intro_page, Gtk::ASSISTANT_PAGE_INTRO);
    this->set_page_title(*this->m_intro_page, _("Introduction"));
    this->set_page_complete(*this->m_intro_page);

    this->append_page(*this->m_class_info_page);
    this->set_page_type(*this->m_class_info_page, Gtk::ASSISTANT_PAGE_CONTENT);
    this->set_page_title(*this->m_class_info_page, _("Class info"));

    this->append_page(*this->m_doc_info_page);
    this->set_page_type(*this->m_doc_info_page, Gtk::ASSISTANT_PAGE_CONTENT);
    this->set_page_title(*this->m_doc_info_page, _("Documentation"));
    this->set_page_complete(*this->m_doc_info_page, this->m_doc_info_page->is_valid());

    this->append_page(*this->m_confirm_page);
    this->set_page_type(*this->m_confirm_page, Gtk::ASSISTANT_PAGE_CONFIRM);
    this->set_page_title(*this->m_confirm_page, _("Confirmation"));

    this->set_title(title);
    this->set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
    this->set_position(Gtk::WIN_POS_CENTER);

    // Signals
    this->m_intro_page->signal_validated().connect(sigc::bind<AssistantPage*>(sigc::mem_fun(*this, &ClassCreatorAssistant::on_page_validated), this->m_intro_page));
    this->m_class_info_page->signal_validated().connect(sigc::bind<AssistantPage*>(sigc::mem_fun(*this, &ClassCreatorAssistant::on_page_validated), this->m_class_info_page));
    this->m_doc_info_page->signal_validated().connect(sigc::bind<AssistantPage*>(sigc::mem_fun(*this, &ClassCreatorAssistant::on_page_validated), this->m_doc_info_page));
    this->m_confirm_page->signal_validated().connect(sigc::bind<AssistantPage*>(sigc::mem_fun(*this, &ClassCreatorAssistant::on_page_validated), this->m_confirm_page));
    this->signal_cancel().connect(sigc::mem_fun(*this, &ClassCreatorAssistant::hide));
    this->signal_close().connect(sigc::mem_fun(*this, &ClassCreatorAssistant::hide));
}

} // ClassCreator
