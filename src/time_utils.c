#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "time_utils.h"


// Returns the Julian Date at 00:00 UT for a Gregorian calendar date.
// Valid for dates after Oct 15, 1582; does not validate its input.
// Algorithm: Meeus, Astronomical Algorithms, ch. 7.
static double gregorian_to_julian(int year, int month, int day)
{
    // start year at march 1
    // jan and feb are 13th and 14th months of previous year
    if (month <= 2) {
        year--;
        month += 12;
    }

    // centuries
    int a = year / 100;

    // -a removes all leap years that land on a century year
    // a/4 gives one back every 4 centuries
    // + 2 aligns to julian date epoch
    int b = 2 - a + a / 4;

    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
}

// Returns days elapsed since J2000 epoch (2451545.0)
double days_since_j2000(int year, int month, int day)
{
    return gregorian_to_julian(year, month, day) - 2451545.0;
}

// from: https://www.benjoffe.com/fast-date
gregdate_t jd_to_greg(double jd)
{
    jd += 2451545.0;
    long long days = (long long)floor(jd + 0.5) - 1721120;

    long long qday = days * 4 + 3;
    long long cent = qday / 146097;
    long long qjul = qday - (cent & ~3) + cent * 4;

    long long year = qjul / 1461;
    long long yday = (qjul % 1461) / 4;

    long long N = yday * 2141 + 197913;
    long long M = N / 65536;
    long long D = N % 65536 / 2141;
    bool bump = (yday >= 306);

    return (gregdate_t){ D + 1, (bump ? M - 12 : M), year + bump };
}