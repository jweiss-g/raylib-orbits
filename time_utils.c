#include <math.h>
#include <stdio.h>

// Returns the Julian Date at 00:00 UT for a Gregorian calendar date.
// Valid for dates after Oct 15, 1582; does not validate its input.
// Algorithm: Meeus, Astronomical Algorithms, ch. 7.
double gregorian_to_julian(int year, int month, int day)
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
