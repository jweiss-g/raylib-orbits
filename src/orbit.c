#include "time_utils.h"
#include "raylib.h"

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
    double semimajor_axis;
    double eccentricity;
    double mean_longitude;
    double perihelion_longitude;

    Color colour;       // raylib Color
} planet_t;

static const planet_t PLANETS[PLANET_COUNT] = {
    // from table 1: https://ssd.jpl.nasa.gov/planets/approx_pos.html
    [MERCURY] = { "Mercury", 0.38709927f, 0.20563593f, 252.25032350f, 77.45779628f },
    [VENUS]   = { "Venus", 0.72333566f, 0.00677672f, 181.97909950f, 131.60246718f },
    [EARTH]   = { "Earth", 1.00000261f, 0.01671123f, 100.46457166f, 102.93768193f },
    [MARS]    = { "Mars", 1.52371034f, 0.09339410f, -4.55343205f, -23.94362959f },
    [JUPITER] = { "Jupiter", 5.20288700f, 0.04838624f, 34.39644051f, 14.72847983f },
    [SATURN]  = { "Saturn", 9.53667594f, 0.05386179f, 49.95424423f, 92.59887831f }, // TODO: render Saturn's ring
    [URANUS]  = { "Uranus", 19.18916464f, 0.04725744f, 313.23810451f, 170.95427630f },
    [NEPTUNE] = { "Neptune", 30.06992276f, 0.00859048f, -55.12002969f, 44.96476227f },
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