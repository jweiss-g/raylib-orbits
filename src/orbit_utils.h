#ifndef ORBITUTIL_H
#define ORBITUTIL_H

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

extern const planet_t PLANETS[PLANET_COUNT];

vec2d_t get_planet_pos(const planet_t *planet, double sim_date);

#endif