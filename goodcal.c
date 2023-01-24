#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <sys/ioctl.h>
#include "utils.h"
#include "appt.h"

int main() {
	initscr(); // Start of ncurses

	// Assigning time vars
	time_t now = time(NULL);
	struct tm* date = localtime(&now);
	int year = date->tm_year;
	int mday = date->tm_mday;
	int wday = date->tm_wday;
	int month = date->tm_mon;
	time_t firstDay = time(NULL) - (86400 * (mday - 1));
	struct tm* firstDate = localtime(&firstDay);
	
	// Regular vars
	int monthDays, keyPress, maxX, maxY;
	int cursorX = 4;
	int cursorY = 3;
	int highlightX = 0;
	int highlightY = 1;
	int columnPrinted = 1;
	// char* apptDest = getenv("HOME");
	// strcat(apptDest, "/.cache/goodcal/apptList");

	// Setting the proper info for based on the date data
	char* currentMonth = setMonthInfo(year + 1900, month, &monthDays);
	
	
	// Preparing to draw
	getmaxyx(stdscr, maxY, maxX);

	// ncurses stuff
	WINDOW* monthWin = newwin(13, 28, 0, 0); // Generating a window to display month info
	WINDOW* apptWin = newwin(20, 50, 0, 29); // Generating a window to display appointment stuff in
	noecho(); // Won't echo the user inputs to the screen
	start_color(); // Enables color
	refresh(); // Actually load in the changes so we can print to the window and stuff
	box(monthWin, 0, 0); // draw a box around the window
	box(apptWin, 0, 0);

	wrefresh(monthWin); // refresh to show the box
	wrefresh(apptWin);
	
	getmaxyx(monthWin, maxY, maxX); // assigns maxY and maxX the limits of the window height and width

	// Printing the appointments currently logged
	printAppointments(&apptWin);

	// Printing the month
	mvwprintw(monthWin, 1, 8, "%s %d", currentMonth, year + 1900);
	mvwprintw(monthWin, 2, 4, "S  M  T  W  T  F  S");
	move(cursorY, cursorX);
	while(true) {
		wmove(monthWin, cursorY, cursorX);
		columnPrinted = 1;
		for(int i = 0; i < monthDays; i++) {
			// Some extra space for if the 1st day of the month isn't on a Sunday
			for(int j = 0; j < firstDate->tm_wday; j++) {
				cursorX += 3;
			}
	
			// reverse the colors for the current day
			if (i + 1 == mday) {
				wattron(monthWin, A_BOLD);
			}
	
			// Printing out the calendar for the month
			if(highlightX == (i - firstDate->tm_wday) % 7 && highlightY == columnPrinted) {
				wattron(monthWin, A_REVERSE);
				mvwprintw(monthWin, cursorY, cursorX, "%d", i + 1);
				wattroff(monthWin, A_REVERSE);
			} else {
				mvwprintw(monthWin, cursorY, cursorX, "%d", i + 1);
			}
	
			// Ending reverse colors if the day printed is the current day
			if (i + 1 == mday) {
				wattroff(monthWin, A_BOLD);
			}
	
			// Some proper Spacing
			cursorX += 3;
	
			// Move down two lines at the end of the week
			if ((firstDate->tm_wday + i + 1) % 7 == 0 && i >= 1) {
				columnPrinted++;
				cursorY += 2;
				cursorX = 4;
				wmove(monthWin, cursorY, cursorX);
			}
		}
	
		// Cursor movement
		keyPress = wgetch(monthWin);
		if(keyPress == 'h') {
			cursorX = 4;
			cursorY = 3;
			highlightX -= 1;
			wrefresh(monthWin);
			refresh();
		}
		if(keyPress == 'l') {
			cursorX = 4;
			cursorY = 3;
			highlightX += 1;
			wrefresh(monthWin);
			refresh();
		}
		if(keyPress == 'j') {
			cursorX = 4;
			cursorY = 3;
			highlightY += 1;
			wrefresh(monthWin);
			refresh();
		}
		if(keyPress == 'k') {
			cursorX = 4;
			cursorY = 3;
			highlightY -= 1;
			wrefresh(monthWin);
			refresh();
		}

		// Making Appointments
		if(keyPress == 'i') {
			promptAppointment(&apptWin);
			wmove(monthWin, 3, 4);
			cursorX = 4;
			cursorY = 3;
			printAppointments(&apptWin);
		}
	
		// End ncurses when hitting 'q'
		if(keyPress == 'q') {
			break;
		}

		// Don't let the cursor go off screen
		if(highlightX == 7) {
			highlightX = 6;
		} else if(highlightX == -1) {
			highlightX = 0;
		}

		if(highlightY == 6) {
			highlightY = 5;
		} else if(highlightY == 0) {
			highlightY = 1;
		}

		refresh();
		wrefresh(monthWin);
	}

	endwin();

	return 0;
}
