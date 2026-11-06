#include "time_utils.h"
#include "raylib.h"

typedef struct {
    float distance;
    float start_angle;
    float period;       // orbit period
    float radius;       // raylib circle radius 
    Color colour;       // raylid Color
} planet_t;

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