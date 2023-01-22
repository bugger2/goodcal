#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "appt.h"

char name[80], startTime[5], endTime[5], group[20];
int charIn;
bool allDay = false;

void promptAppointment(WINDOW** apptWin) {
	echo();
	wmove(*apptWin, 1, 1);

	wprintw(*apptWin, "Appointment Name: ");
	wgetstr(*apptWin, name);

	mvwprintw(*apptWin, 2, 1, "All Day? [y/n]: ");
	charIn = wgetch(*apptWin);
	if (charIn == 'n') {
		mvwprintw(*apptWin, 3, 1, "Start Time (Army Format): ");
		wgetstr(*apptWin, startTime);
		mvwprintw(*apptWin, 4, 1, "End Time (Army Format): ");
		wgetstr(*apptWin, endTime);
	} else {
		allDay = true;
	}

	mvwprintw(*apptWin, 5, 1, "Group: ");
	wgetstr(*apptWin, group);

	noecho();
}
