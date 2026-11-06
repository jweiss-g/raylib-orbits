#include "time_utils.h"
#include "raylib.h"
#include <math.h>

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

static const planet_t PLANETS[PLANET_COUNT] = {
    // from table 1: https://ssd.jpl.nasa.gov/planets/approx_pos.html
    [MERCURY] = { "Mercury", 0.38709927f, 0.20563593f, 252.25032350f, 77.45779628f, 
        0.00000037f, 0.00001906f, 149472.67411175f, 0.16047689f },

    [VENUS]   = { "Venus", 0.72333566f, 0.00677672f, 181.97909950f, 131.60246718f, 
        0.00000390f, -0.00004107f, 58517.81538729f, 0.00268329f },

    [EARTH]   = { "Earth", 1.00000261f, 0.01671123f, 100.46457166f, 102.93768193f, 
        0.00000562f, -0.00004392f, 35999.37244981f, 0.32327364f },

    [MARS]    = { "Mars", 1.52371034f, 0.09339410f, -4.55343205f, -23.94362959f, 
        0.00001847f, 0.00007882f, 19140.30268499, 0.44441088f },

    [JUPITER] = { "Jupiter", 5.20288700f, 0.04838624f, 34.39644051f, 14.72847983f, 
        -0.00011607f, -0.00013253f, 3034.74612775f, 0.21252668f },
        
    [SATURN]  = { "Saturn", 9.53667594f, 0.05386179f, 49.95424423f, 92.59887831f, 
        -0.00125060f, -0.00050991f, 1222.49362201f, -0.41897216f },

    [URANUS]  = { "Uranus", 19.18916464f, 0.04725744f, 313.23810451f, 170.95427630f, 
        -0.00196176f, -0.00004397f, 428.48202785f, 0.40805281f },

    [NEPTUNE] = { "Neptune", 30.06992276f, 0.00859048f, -55.12002969f, 44.96476227f, 
        0.00026291f, 0.00005105f, 0.00005105f, -0.32241464f },

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