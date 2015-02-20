/* ========================================================================
   File: day_of_year.cpp
   Date: Feb. 20/15
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   day_of_year and month_day from K&R re-written to use pointers instead of
   indexing.
   ======================================================================== */

#include <stdio.h>
#include <cstdint>

static uint8_t normalDayTab[] = 
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static uint8_t leapDayTab[] = 
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// NOTE(brendan): 2-D array of leap-year and non-leap-year days per month
static uint8_t *daytab[2] = {normalDayTab, leapDayTab};

// NOTE(brendan): set day of year from month & day
int day_of_year(int year, int month, int day) {
  int leap = ((year%4 == 0) && (year%100 != 0)) || (year%400 == 0);
  for(int i = 1; i < month; ++i) {
    day += daytab[leap][i];
  }
  return day;
}

// NOTE(brendan): set month, day from day of year
void month_day(int year, int yearday, int *pmonth, int *pday) {
  int leap = ((year%4 == 0) && (year%100 != 0)) || (year%400 == 0);
  int i = 1;
  while(yearday > daytab[leap][i]) {
    yearday -= daytab[leap][i++];
  }
  *pmonth = i;
  *pday = yearday;
}

int main(int argc, char *argv[]) {
  int month, day;
  month_day(1988, 365, &month, &day);
  printf("Month: %d Day: %d\n", month, day_of_year(1988, month, day));
}
