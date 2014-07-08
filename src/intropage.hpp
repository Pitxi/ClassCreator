/**
 * @file
 * IntroPage class declaration.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright 2014 Javier Campón Pichardo
 *
 * Distributed under the terms of the GPL version 3 license.
 */

#ifndef INTROPAGE_HPP
#define INTROPAGE_HPP

#include "assistantpage.hpp"

/**
 * ClassCreator namespace.
 */
namespace ClassCreator
{

/**
 * The Introduction page for the class creator assistant.
 */
class IntroPage final : public AssistantPage
{
private:
    void on_preferences_button_clicked();
    bool validate_page();

public:
    IntroPage();
};

} // ClassCreator
#endif // INTROPAGE_HPP
