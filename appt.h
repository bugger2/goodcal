#include <ncurses.h>

#ifndef APPT_H
#define APPT_H

void promptAppointment(WINDOW **apptWin, char *monthName, int monthDay,
                       int year, int monthInt);
void printAppointments(WINDOW **apptWin);

#endif
