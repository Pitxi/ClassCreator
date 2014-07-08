/**
 * @file
 * Implementación de la clase DatePicker.
 * @author Javier Campón Pichardo
 * @date 2014
 * @copyright GNU Public License Version 3
 */

#include "datepicker.h"
#include <gtkmm/grid.h>

/**
 * Espacio de nombres de la aplicación.
 */
namespace ClassCreator
{

/**
 * Actualiza el texto del objeto con la fecha escogida usando el formato
 * asociado al mismo.
 */
void DatePicker::updateText()
{
    this->set_label(this->getDate().format_string(this->getFormat()));
}

/**
 * Muestra la ventana emergente con el calendario.
 */
void DatePicker::showCalendar()
{
    this->m_popup.hide();
    if (this->get_active()) {
        int x, y;
        this->get_window()->get_position(x, y);
        this->m_popup.move(x + this->get_allocation().get_x(), y + this->get_allocation().get_y() + this->get_height());
        this->m_popup.show_all();
    }
}

/**
 * Cierra el calendario si el objeto pierde el foco.
 * @param event Evento GDK de foco.
 * @return Devuelve siempre (@c FALSE) para continuar la propagación del evento.
 */
bool DatePicker::onFocusOut(GdkEventFocus *event)
{
    this->set_active(false);
    return false;
}

/**
 * Constructor.
 * @param format Formato con el que se mostrará la fecha escogida en el objeto,
 * siguiendo las reglas de la función C strftime.
 */
DatePicker::DatePicker(const Glib::ustring &format) :
    m_popup(Gtk::WINDOW_POPUP)
{
    this->m_calendar = Gtk::manage(new Gtk::Calendar);

    this->setFormat(format);
    this->set_alignment(0.5, 0.5);
    this->updateText();

    this->m_popup.add(*this->m_calendar);

    this->signal_toggled().connect(sigc::mem_fun(*this, &DatePicker::showCalendar));
    this->m_calendar->signal_day_selected().connect(sigc::mem_fun(*this, &DatePicker::updateText));
    this->signal_focus_out_event().connect(sigc::mem_fun(*this, &DatePicker::onFocusOut));
}

/**
 * Obtiene el formato con el que se representa la fecha escogida en el
 * DatePicker.
 * @return Format de representación de la fecha, siguiendo las reglas de la
 * función C strftime.
 */
const Glib::ustring &DatePicker::getFormat() const
{
    return this->m_format;
}

/**
 * Establece el formato de la fecha representada en el DatePicker.
 * @param format Nuevo formato de fecha, siguiendo las reglas de la función C
 * strftime.
 */
void DatePicker::setFormat(const Glib::ustring &format)
{
    this->m_format = format;
    this->updateText();
}

/**
 * Obtiene la fecha del DatePicker.
 * @return Fecha actualmente seleccionada en el DatePicker.
 */
Glib::Date DatePicker::getDate() const
{
    Glib::Date date;
    this->m_calendar->get_date(date);

    return date;
}

/**
 * Establece la fecha indicada en el DatePicker.
 * @param date Nueva fecha.
 */
void DatePicker::setDate(const Glib::Date &date)
{
    this->m_calendar->select_day(date.get_day());
    this->m_calendar->select_month(date.get_month(), date.get_year());
}

} // ClassCreator
