// -*- tab-width: 4; c-basic-offset: 4 -*-
#include "appt.h"
#include "utils.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    initscr(); // Start of ncurses

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
    int monthDays, maxX, maxY;
    // cursor represents where the ncurses cursor is, where the program is drawing
    // highlight represents where the cursor that the user controls is, the white
    // block you move around
    int cursorX = 4;    // X position for the cursor
    int cursorY = 3;    // Y position for the cursor
    int highlightX = 4; // X position for the cursor that the user controls
    int highlightY = 2; // Y position for the cursor that the user controls
    int rowPrinted = 1; // Tracks how many rows we have printed
    // char* apptDest = getenv("HOME");
    // strcat(apptDest, "/.cache/goodcal/apptList");

    // Setting the proper info for based on the date data
    char *currentMonth = setMonthInfo(year + 1900, month, &monthDays);

    // ncurses stuff
    WINDOW *monthWin =
        newwin(15, 28, 0, 0); // The window the calendar will appear in
    WINDOW *apptWin =
        newwin(20, 50, 0, 29); // The window the appointments will appear in
    noecho();                  // Won't echo the user inputs to the screen
    start_color();             // Enables color
    refresh(); // Actually load in the changes so we can print to the window and
                // stuff
    box(monthWin, 0, 0); // draw a box around the window
    box(apptWin, 0, 0);

    wrefresh(monthWin); // refresh to show the box
    wrefresh(apptWin);

    getmaxyx(
        monthWin, maxY,
        maxX); // assigns maxY and maxX the limits of the window height and width

    // Printing the appointments currently logged
    // printAppointments(&apptWin);

    // Printing the month
    mvwprintw(monthWin, 1, 8, "%s %d", currentMonth, year + 1900);
    mvwprintw(monthWin, 2, 4, "Su Mo Tu We Th Fr Sa");
    wmove(monthWin, cursorY, cursorX);
    while (true) {
        wmove(monthWin, cursorY, cursorX);
        moveForSunday(firstDate->tm_wday,
                      &cursorX); // Some extra space for if the 1st day of the month
                                 // isn't on a Sunday
        rowPrinted = 1;
        for (int i = 0; i < monthDays; i++) {

            // debug messages
            mvwprintw(monthWin, 2, 1, "%d",
                    (42 - monthDays - firstDate->tm_wday) % 7);

            // Make the current day standout
            if (i + 1 == mday) {
                wattron(monthWin, A_BOLD);
            }

            // Printing out the calendar for the month
            if ((highlightX == i % 7 || (highlightX == 0 - firstDate->tm_wday &&
                                       (i + firstDate->tm_wday) % 7 == 0)) &&
                (highlightY == rowPrinted)) {
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
            if (cursorX >= 25) {
                rowPrinted++;
                cursorY += 2;
                cursorX = 4;
                wmove(monthWin, cursorY, cursorX);
            }
        }

        /* Key presses */
        int keyPress = wgetch(monthWin);

        // As long as we aren't about to move to an empty space, move the
        // highlighted cursor to the left
        if (keyPress == 'h' && !(highlightX == 0 && highlightY == 1)) {
            refreshCal(firstDate->tm_wday, &cursorX, &cursorY, &highlightX, &monthWin,
                       -1);
        }

        if (keyPress == 'l' &&
            !(highlightX > (40 - monthDays - firstDate->tm_wday) % 7 &&
                (highlightY == 5 + (monthDays + firstDate->tm_wday < 35 ? 0 : 1)))) {
            refreshCal(firstDate->tm_wday, &cursorX, &cursorY, &highlightX, &monthWin,
                      1);
        }

        if (keyPress == 'j' &&
            !(highlightX > (41 - monthDays - firstDate->tm_wday) % 7 &&
                highlightY == ((monthDays + firstDate->tm_wday > 35) ? 3 : 4))) {
            refreshCal(firstDate->tm_wday, &cursorX, &cursorY, &highlightY, &monthWin,
                      1);
        }

        if (keyPress == 'k' &&
            !(highlightX + firstDate->tm_wday == 0 && highlightY == 2)) {
            refreshCal(firstDate->tm_wday, &cursorX, &cursorY, &highlightY, &monthWin,
                      -1);
        //} else if(keyPress == 'i') { // Making Appointments */
            // promptAppointment(&apptWin, currentMonth, highlightX + ((highlightY -
            // 1) * 7) - firstDate->tm_wday + 1, year + 1900, month);
            // wmove(monthWin, 3, 4); cursorX = 4; cursorY = 3;
            // printAppointments(&apptWin);
        } else if (keyPress == 'q') { // End ncurses when hitting 'q'
            break;
        } else {
            wmove(monthWin, 3, 4);
            cursorY = 3;
            cursorX = 4;
        }

        // Don't let the cursor go off screen
        if (highlightX == 7 - firstDate->tm_wday) {
            highlightX = 6 - firstDate->tm_wday;
        } else if (highlightX == -1 - firstDate->tm_wday) {
            highlightX = 0 - firstDate->tm_wday;
        }

        if (highlightY == 6 && firstDate->tm_wday + monthDays < 35) {
            highlightY = 5;
        } else if (highlightY == 7 && firstDate->tm_wday + monthDays >= 35) {
            highlightY = 6;
        } else if (highlightY == 0) {
            highlightY = 1;
        }

        refresh();
        wrefresh(monthWin);
    }

    endwin();

    return 0;
}
