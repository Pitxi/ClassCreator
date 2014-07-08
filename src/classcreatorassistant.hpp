/**
 * @file
 * ClassCreatorAssistant class declaration.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#ifndef CLASSCREATORASSISTANT_HPP
#define CLASSCREATORASSISTANT_HPP

#include "assistantpage.hpp"
#include "intropage.hpp"
#include "classinfopage.hpp"
#include "docinfopage.hpp"
#include "confirmpage.hpp"
#include <gtkmm/assistant.h>

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * Assistant for new C++ classes creation.
 */
class ClassCreatorAssistant final : public Gtk::Assistant
{
private:
    IntroPage *m_intro_page          = nullptr;
    ClassInfoPage *m_class_info_page = nullptr;
    DocInfoPage *m_doc_info_page     = nullptr;
    ConfirmPage *m_confirm_page      = nullptr;
    Glib::RefPtr<Gio::Settings> m_settings;

    void on_page_validated(bool valid, AssistantPage *sender);
    void on_prepare(Gtk::Widget *page);
    void on_apply();

public:
    ClassCreatorAssistant(const Glib::ustring &title);
};

} // ClassCreator

#endif // CLASSCREATORASSISTANT_HPP
