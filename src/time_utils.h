#ifndef TIMEUTIL_H
#define TIMEUTIL_H

typedef struct {
    int day;
    int month;
    int year;
} gregdate_t;

double days_since_j2000(int year, int month, int day);
gregdate_t jd_to_greg(double jd);

#endif