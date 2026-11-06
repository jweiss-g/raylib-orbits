#include "raylib.h"
#include "time_utils.h"
#include "orbit_utils.h"
#include <math.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
    double x;
    double y;
    int radius;
    Color colour;
} rendered_planet_t;


int main(void)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;
    double max_radius = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.0 - 20;
    double au_scale = max_radius / sqrt(PLANETS[NEPTUNE].semimajor_axis);

    rendered_planet_t rendered_planets[PLANET_COUNT] = {
        [MERCURY] = { 0, 0, 4, GRAY },
        [VENUS]   = { 0, 0, 4, BEIGE },
        [EARTH]   = { 0, 0, 6, SKYBLUE },
        [MARS]    = { 0, 0, 6, MAROON },
        [JUPITER] = { 0, 0, 12, BEIGE },
        [SATURN]  = { 0, 0, 10, BEIGE },
        [URANUS]  = { 0, 0, 8, BLUE },
        [NEPTUNE] = { 0, 0, 8, DARKBLUE },
    };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "orbits");

    vec2d_t sun_pos = { (double)SCREEN_WIDTH / 2.0, (double)SCREEN_HEIGHT / 2.0 };

    SetTargetFPS(60);

    double days_per_second = 30.0f;                     // sim time scale
    double sim_date = days_since_j2000(2000, 1, 1);
    Vector2 vertices[128];

    while (!WindowShouldClose())
    {
        sim_date += GetFrameTime() * days_per_second;
        BeginDrawing();
            DrawText(TextFormat("Elapsed Time: %.2f", sim_date), 10, 10, 12, GRAY);
            ClearBackground(BLACK);
            DrawCircle(sun_pos.x, sun_pos.y, 20, YELLOW);
            for (int i = 0; i < PLANET_COUNT; i++) {
                for (int j = 0; j <= 128; j++) {
                    double tmp = (2*PI * j) / 128;
                    vec2d_t tmp_v = to_vec(&PLANETS[i], sim_date, tmp);
                    double distance = hypot(tmp_v.x, tmp_v.y);
                    double scalar = au_scale / sqrt(distance);
                    double x = sun_pos.x + scalar * tmp_v.x;
                    double y = sun_pos.y - scalar * tmp_v.y;
                    Vector2 v = { x, y };
                    vertices[j] = v;
                }
                DrawLineStrip(vertices, 129, GRAY);
            }
            for (int i = 0; i < PLANET_COUNT; i++) {
                rendered_planet_t p = rendered_planets[i]; 
                vec2d_t v = get_planet_pos(&PLANETS[i], sim_date);
                double distance = hypot(v.x, v.y);
                double scalar = au_scale / sqrt(distance);
                p.x = sun_pos.x + scalar * v.x;
                p.y = sun_pos.y - scalar * v.y;
                DrawCircle(p.x, p.y, p.radius, p.colour);
            }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}