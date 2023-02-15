#include <stdlib.h>
#include <ncurses.h>
#include "utils.h"

char* setMonthInfo(int year, int month, int* monthDays) {
	char* monthName = NULL;

	if (month == 0) {
		monthName = "January";
		*monthDays = 31;
	} else if (month == 1){
		if (((year + 1900) % 4) == 0) {
			*monthDays = 29;
		} else {
			*monthDays = 28;
		}
		monthName = "February";
	} else if ( month == 2) {
		monthName = "March";
		*monthDays = 31;
	} else if (month == 3) {
		monthName = "April";
		*monthDays = 30;
	} else if (month == 4) {
		monthName = "May";
		*monthDays = 31;
	} else if (month == 5) {
		monthName = "June";
		*monthDays = 30;
	} else if (month == 6) {
		monthName = "July";
		*monthDays = 31;
	} else if (month == 7) {
		monthName = "August";
		*monthDays = 31;
	} else if (month == 8) {
		monthName = "September";
		*monthDays = 30;
	} else if (month == 9) {
		monthName = "October";
		*monthDays = 31;
	} else if (month == 10) {
		monthName = "November";
		*monthDays = 30;
	} else if (month == 11) {
		monthName = "December";
		*monthDays = 31;
	}

	return monthName;
}

void sort(int array[], int size, int orderArray[]) {
	for(int i = 0; i < size - 1; i++) {
		for(int j = 0; j < size - i - 1; j++) {
			if(array[j] > array[j + 1]) {
				int temp = array[j];
				int orderTemp = orderArray[j];
				array[j] = array[j + 1];
				orderArray[j] = orderArray[j + 1];
				array[j + 1] = temp;
				orderArray[j + 1] = orderTemp;
			}
		}
	}
}

void moveForSunday(int daysToMove, int* cursorX) {
	for(int i = 0; i < daysToMove; i++) {
		*cursorX += 3;
	}
}

void refreshCal(int daysToMove, int* cursorX, int* cursorY, int* highlight, WINDOW** win, int multiplier) {
	*cursorX = 4;
	*cursorY = 3;
	*highlight += (1 * multiplier);
	moveForSunday(daysToMove, cursorX);
	wrefresh(*win);
	refresh();
}
