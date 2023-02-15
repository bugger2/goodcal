#ifndef UTILS_H
#define UTILS_H

char* setMonthInfo(int year, int month, int* monthDays);
void sort(int array[], int size, int dateArray[]);
void moveForSunday(int daysToMove, int* cursorX);
void refreshCal(int datsToMove, int* cursorX, int* cursorY, int* highlight, WINDOW** win, int multiplier);

#endif
