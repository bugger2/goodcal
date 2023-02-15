#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "appt.h"
#include "utils.h"

char name[80] = { " " } , startTime[5] = { " " } , endTime[5] = { " " };
int charIn;
int cursorY = 1;
char apptInfo [100];
FILE* apptFile;
char monthDayString[2];
char apptAmountBuffer[4];

void promptAppointment(WINDOW** apptWin, char* monthName, int monthDay, int year, int monthInt) {
	printf("%d", monthDay);
	wclear(*apptWin);
	box(*apptWin, 0, 0);
	wrefresh(*apptWin);
	apptFile = fopen("apptList", "r");
	if(fgetc(apptFile) == EOF) {
		apptFile = fopen("apptList", "a");
		fprintf(apptFile, " ");
	} else {
		apptFile = fopen("apptList", "a");
		fprintf(apptFile, "\n");
	}
	fprintf(apptFile, "%d", year);
	fprintf(apptFile, "%d", monthInt);
	sprintf(monthDayString, "%d", monthDay);
	fprintf(apptFile, monthDayString);
	echo();
	wmove(*apptWin, 1, 1);

	wprintw(*apptWin, "Appointment Name: ");
	wgetstr(*apptWin, name);
	fprintf(apptFile, name);

	mvwprintw(*apptWin, 2, 1, "All Day? [y/n]: ");
	charIn = wgetch(*apptWin);
	if (charIn == 'n') {
		fprintf(apptFile, " @ ");
		mvwprintw(*apptWin, 3, 1, "Start Time: ");
		wgetstr(*apptWin, startTime);
		fprintf(apptFile, startTime);
		fprintf(apptFile, " - ");
		mvwprintw(*apptWin, 4, 1, "End Time: ");
		wgetstr(*apptWin, endTime);
		fprintf(apptFile, endTime);
	}
	fprintf(apptFile, "\n");
	fclose(apptFile);

	noecho();
}

void printAppointments(WINDOW** apptWin) {
	apptFile = fopen("apptAmount", "r");
	cursorY = 1;
	wmove(*apptWin, cursorY, 1);
	wclear(*apptWin);

	// Reading the dates for each appointment
	fgets(apptAmountBuffer, 4, apptFile);
	int datesList[atoi(apptAmountBuffer)];
	int datesOrder[atoi(apptAmountBuffer)];

	char dateChar = fgetc(apptFile); // Sequentially reads the the date
	for(int i = 0; i < atoi(apptAmountBuffer); i++) {
		datesList[i] = dateChar - '0';
		//for(int j = 0; j < 4; j++) {
			//dateCharToStr[j] = fgetc(apptFile);
		//}
		for(int j = 0; j < 8; j++) {
			datesList[i] = 10 * datesList[i] + (dateChar - '0');
			printf("%d", datesList[i]);
		}
	}

	// Sort the appointments by date
	sort(datesList, atoi(apptAmountBuffer), datesOrder);

	apptFile = fopen("apptList", "r");
	while(fgetc(apptFile) != EOF) {
		for(int i = 0; i < 8; i++) { // Skip over the date format
			fgetc(apptFile);
		}

		for(int i = 0; i < datesOrder[i] - 1; i++) {
			fgets(apptInfo, 100, apptFile);
			mvwprintw(*apptWin, cursorY++, 1, "%s", apptInfo);
		}
	}
	box(*apptWin, 0, 0);
	wrefresh(*apptWin);
	fclose(apptFile);
}
