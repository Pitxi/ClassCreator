/**
 * @file
 * Declaración de la clase DatePicker.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <gtkmm/togglebutton.h>
#include <gtkmm/calendar.h>
#include <gtkmm/entry.h>
#include <gtkmm/window.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Date picker widget.
 */
class DatePicker : public Gtk::ToggleButton
{
private:
    Gtk::Calendar *m_calendar;
    Gtk::Window m_popup;
    Glib::ustring m_format;

protected:
    void updateText();
    void showCalendar();
    bool onFocusOut(GdkEventFocus *event);

public:
    DatePicker(const Glib::ustring &format = "%Ec");
    virtual ~DatePicker() {}
    const Glib::ustring &getFormat() const;
    void setFormat(const Glib::ustring &format);
    Glib::Date getDate() const;
    void setDate(const Glib::Date &date);
};

} // ClassCreator

#endif // DATEPICKER_H
