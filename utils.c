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
