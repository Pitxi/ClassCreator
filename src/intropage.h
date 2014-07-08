/**
 * @file
 * Declaración de la clase IntroPage.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef INTROPAGE_H
#define INTROPAGE_H

#include "assistantpage.h"

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Página de introducción del asistente.
 */
class IntroPage : public AssistantPage
{
protected:
    void onPreferencesClicked();

public:
    IntroPage();
    virtual ~IntroPage() {}
};

} // ClassCreator

#endif // INTROPAGE_H
