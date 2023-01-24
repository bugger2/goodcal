#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "appt.h"

char name[80] = { " " } , startTime[5] = { " " } , endTime[5] = { " " } , group[20] = { " " };
int charIn;
int cursorY = 1;
char apptInfo [100];
FILE* aApptFile;
FILE* rApptFile;

void promptAppointment(WINDOW** apptWin) {
	wclear(*apptWin);
	box(*apptWin, 0, 0);
	wrefresh(*apptWin);
	aApptFile = fopen("apptList", "a");
	fprintf(aApptFile, " ");
	echo();
	wmove(*apptWin, 1, 1);

	fprintf(aApptFile, "\n ");
	wprintw(*apptWin, "Appointment Name: ");
	wgetstr(*apptWin, name);
	fprintf(aApptFile, name);

	mvwprintw(*apptWin, 2, 1, "All Day? [y/n]: ");
	charIn = wgetch(*apptWin);
	if (charIn == 'n') {
		fprintf(aApptFile, " @ ");
		mvwprintw(*apptWin, 3, 1, "Start Time: ");
		wgetstr(*apptWin, startTime);
		fprintf(aApptFile, startTime);
		fprintf(aApptFile, " - ");
		mvwprintw(*apptWin, 4, 1, "End Time: ");
		wgetstr(*apptWin, endTime);
		fprintf(aApptFile, endTime);
	}
	fclose(aApptFile);

	noecho();
}

void printAppointments(WINDOW** apptWin) {
	rApptFile = fopen("apptList", "r");
	cursorY = 1;
	wmove(*apptWin, cursorY, 1);
	wclear(*apptWin);
	while(fgetc(rApptFile) != EOF) {
		fgets(apptInfo, 100, rApptFile);
		mvwprintw(*apptWin, cursorY++, 1, "%s", apptInfo);
		printf("%s", apptInfo);
	}
	box(*apptWin, 0, 0);
	wrefresh(*apptWin);
	fclose(rApptFile);
}
