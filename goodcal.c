#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <sys/ioctl.h>
#include "utils.h"

// Function that takes some struct tm* data and makes it good for displaying to the user

int main() {

	// Assigning time vars
	time_t now = time(NULL);
	struct tm *date = localtime(&now);
	int year = date->tm_year;
	int mday = date->tm_mday;
	int wday = date->tm_wday;
	int month = date->tm_mon;
	time_t firstDay = time(NULL) - (86400 * (mday - 1));
	struct tm *firstDate = localtime(&firstDay);

	// Regular vars
	int monthDays;

	// Setting the proper info for based on the date data
	char* currentMonth = setMonthInfo(year + 1900, month, &monthDays);

	initscr(); // Start of ncurses

	// Some stuff for proper spacing
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	int cursorX = (w.ws_col / 2) - 14;
	int cursorY = (w.ws_row / 2) - 3;

	WINDOW* win = newwin(11, 28, cursorY, cursorX); // Generating a window to display things in
	noecho(); // Won't echo the user inputs to the screen
	start_color(); // Enables color
	move(cursorY, cursorX); // Move the cursor to the center of the terminal
	refresh(); // Actually load in the changes so we can print to the window and stuff

	wprintw(win, "    %s %d\n", currentMonth, year + 1900);
	wprintw(win, "S  M  T  W  T  F  S\n");
	for(int i = 0; i < monthDays; i++) {
		// Some extra space for if the 1st day of the month isn't on a Sunday
		for(int j = 0; j < firstDate->tm_wday; j++) {
			wprintw(win, "   ");
		}

		// reverse the colors for the current day
		if (i + 1 == mday) {
			wattron(win, A_REVERSE);
		}

		// Printing out the calendar for the month
		wprintw(win, "%d", i + 1);

		// Ending reverse colors if the day printed is the current day
		if (i + 1 == mday) {
			wattroff(win, A_REVERSE);
		}

		// Some proper Spacing
		if (i + 1 <= 9) {
			wprintw(win, "  ");
		} else {
			wprintw(win, " ");
		}

		if ((firstDate->tm_wday + i + 1) % 7 == 0 && i >= 1) {
			wprintw(win, "\n\n");
		}
	}
	refresh();
	wrefresh(win);

	while(true) {
		// Cursor movement
		if(getch() == (int)'h') {
			move(cursorY, --cursorX);
			wrefresh(win);
			refresh();
		}
		if(getch() == (int)'l') {
			move(cursorY, ++cursorX);
			wrefresh(win);
			refresh();
		}
		if(getch() == (int)'j') {
			move(++cursorY, cursorX);
			wrefresh(win);
			refresh();
		}
		if(getch() == (int)'k') {
			move(--cursorY, cursorX);
			wrefresh(win);
			refresh();
		}
	
		// End ncurses when hitting 'q'
		if(getch() == (int)'q') {
			endwin();
		}
	}

	refresh();
	return 0;
}
