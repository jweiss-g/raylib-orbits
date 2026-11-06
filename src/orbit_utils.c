#include "raylib.h"
#include <math.h>

static const double JULIAN_CENTURY = 36525.0f;

typedef enum {
    MERCURY, 
    VENUS, 
    EARTH, 
    MARS, 
    JUPITER, 
    SATURN, 
    URANUS, 
    NEPTUNE, 
    PLANET_COUNT,
} planet_id;

typedef struct {
    const char* name;
    // keplerian elements required for 2d render
    double semimajor_axis;              // AU
    double eccentricity;                // radians
    double mean_longitude;              // degrees
    double perihelion_longitude;        // degrees

    double semimajor_axis_rate;         // AU/Cy
    double eccentricity_rate;           // rad/Cy
    double mean_longitude_rate;         // deg/Cy
    double perihelion_longitude_rate;   // deg/Cy
} planet_t;

typedef struct {
    double x, y;
} vec2d_t;

static const planet_t PLANETS[PLANET_COUNT] = {
    // from table 1: https://ssd.jpl.nasa.gov/planets/approx_pos.html
    [MERCURY] = { "Mercury", 0.38709927, 0.20563593, 252.25032350, 77.45779628, 
        0.00000037, 0.00001906, 149472.67411175, 0.16047689 },

    [VENUS]   = { "Venus", 0.72333566, 0.00677672, 181.97909950, 131.60246718, 
        0.00000390, -0.00004107, 58517.81538729, 0.00268329 },

    [EARTH]   = { "Earth", 1.00000261, 0.01671123, 100.46457166, 102.93768193, 
        0.00000562, -0.00004392, 35999.37244981, 0.32327364 },

    [MARS]    = { "Mars", 1.52371034, 0.09339410, -4.55343205, -23.94362959, 
        0.00001847, 0.00007882, 19140.30268499, 0.44441088 },

    [JUPITER] = { "Jupiter", 5.20288700, 0.04838624, 34.39644051, 14.72847983, 
        -0.00011607, -0.00013253, 3034.74612775, 0.21252668 },
        
    [SATURN]  = { "Saturn", 9.53667594, 0.05386179, 49.95424423, 92.59887831, 
        -0.00125060, -0.00050991, 1222.49362201, -0.41897216 },

    [URANUS]  = { "Uranus", 19.18916464, 0.04725744, 313.23810451, 170.95427630, 
        -0.00196176, -0.00004397, 428.48202785, 0.40805281 },

    [NEPTUNE] = { "Neptune", 30.06992276, 0.00859048, -55.12002969, 44.96476227, 
        0.00026291, 0.00005105, 218.45945325, -0.32241464 },

    // TODO: add dwarf planets (or just pluto) separately as toggle?
};

/**
 * @brief Approximates eccentric anomaly using Kepler's equation
 * 
 * M = E - e(sin(E))
 * 
 * @param M  Mean anomaly
 * @param e  Orbital eccentricity
 * @return E Eccentric anomaly in radians
 */
double solve_kepler(double M, double e)
{
    // initial guess
    double E = M;       
    for (int i = 0; i < 5; i++) {
        E -= (E - e*sin(E) - M) / (1 - e*cos(E));
    }
    return E;
}

// returns a planet's (x, y) position in AU at a specified time
vec2d_t get_planet_pos(const planet_t *planet, double sim_date)
{
    double centuries = sim_date / JULIAN_CENTURY;

    // calculate new values using rates of change over julian century
    double majr_axis = planet->semimajor_axis + (planet->semimajor_axis_rate * centuries);
    double ecc = planet->eccentricity + (planet->eccentricity_rate * centuries);
    double mean_long = (planet->mean_longitude + (planet->mean_longitude_rate * centuries)) *DEG2RAD;
    double peri_long = (planet->perihelion_longitude + (planet->perihelion_longitude_rate * centuries)) *DEG2RAD;

    // get mean anomaly, eccentric anomaly, and minor axis from updated values
    double mean_anomaly = fmod(mean_long - peri_long, 2.0f * PI); // or remainder()???
    double ecc_anomaly = solve_kepler(mean_anomaly, ecc);
    double minr_axis = majr_axis * sqrt(1 - pow(ecc, 2));

    double x_prm = majr_axis * (cos(ecc_anomaly) - ecc);
    double y_prm = minr_axis * sin(ecc_anomaly);

    // updated x and y positions
    vec2d_t v;
    v.x = x_prm * cos(peri_long) - y_prm * sin(peri_long);
    v.y = x_prm * sin(peri_long) + y_prm * cos(peri_long);
    return v;
}