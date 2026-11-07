#include "raylib.h"
#include "time_utils.h"
#include "orbit_utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define YEAR_CHARS 4
#define MONTH_CHARS 2
#define DAY_CHARS 2

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
    int radius;
    Color colour;
} rendered_planet_t;

static const rendered_planet_t rendered_planets[PLANET_COUNT] = {
    [MERCURY] = { 4, GRAY },
    [VENUS]   = { 4, BEIGE },
    [EARTH]   = { 6, SKYBLUE },
    [MARS]    = { 6, MAROON },
    [JUPITER] = { 12, BEIGE },
    [SATURN]  = { 10, BEIGE },
    [URANUS]  = { 8, BLUE },
    [NEPTUNE] = { 8, DARKBLUE },
};

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 800;
static const double max_radius = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.0 - 20;
static const vec2d_t sun_pos = { (double)SCREEN_WIDTH / 2.0, (double)SCREEN_HEIGHT / 2.0 };

vec2d_t au_to_px(vec2d_t v);
int clamp_day(int month, int day);

int main(void)
{

    int year = 2000;
    int month = 1;
    int day = 1;

    Rectangle year_box  = { 45, SCREEN_HEIGHT - 35, 50, 25 };
    Rectangle month_box = { 105, SCREEN_HEIGHT - 35, 25, 25 };
    Rectangle day_box   = { 140, SCREEN_HEIGHT - 35, 25, 25 };

    bool is_paused = false;
    bool btn_pause_pressed = false; 

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "orbits");


    SetTargetFPS(60);

    double days_per_second = 0.5f;                     // sim time scale
    double sim_date = days_since_j2000(year, month, day);
    Vector2 vertices[128];

    bool edit_year = false;
    bool edit_month = false;
    bool edit_day = false;

    while (!WindowShouldClose())
    {
        if (btn_pause_pressed) {
            is_paused = !is_paused;
        }

        if (!is_paused) {
            sim_date += GetFrameTime() * days_per_second;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawCircle(sun_pos.x, sun_pos.y, 20, YELLOW);

            // draw planet orbits
            for (int i = 0; i < PLANET_COUNT; i++) {
                for (int j = 0; j <= 128; j++) {
                    double tmp = (2*PI * j) / 128;
                    vec2d_t tmp_v = to_vec(&PLANETS[i], sim_date, tmp);
                    tmp_v = au_to_px(tmp_v);
                    Vector2 v = { tmp_v.x, tmp_v.y };
                    vertices[j] = v;
                }
                DrawLineStrip(vertices, 129, GRAY);
            }

            // draw planets
            for (int i = 0; i < PLANET_COUNT; i++) {
                rendered_planet_t p = rendered_planets[i]; 
                vec2d_t v = get_planet_pos(&PLANETS[i], sim_date);
                v = au_to_px(v);
                DrawCircle(v.x, v.y, p.radius, p.colour);
            }

            btn_pause_pressed = GuiButton((Rectangle){ 10, SCREEN_HEIGHT - 35, 25, 25}, "#132#");

            if (GuiValueBox(year_box, NULL, &year, 1, 9999, edit_year)) {
                if (is_paused) {
                    if (edit_year) sim_date = days_since_j2000(year, month, day);
                    edit_year = !edit_year;
                }
            }

            if (GuiValueBox(month_box, NULL, &month, 1, 12, edit_month)) {
                if (is_paused) {
                    if (edit_month) {
                        day = clamp_day(month, day);
                        sim_date = days_since_j2000(year, month, day);
                    }
                    edit_month = !edit_month;
                }
            }

            if (GuiValueBox(day_box, NULL, &day, 1, 31, edit_day)) {
                if (is_paused) {
                    if (edit_day) {
                        day = clamp_day(month, day);
                        sim_date = days_since_j2000(year, month, day);
                    }
                    edit_day = !edit_day;
                }
            }

            if (!is_paused) {
                edit_year = false;
                edit_month = false;
                edit_day = false;
            }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

vec2d_t au_to_px(vec2d_t v)
{
    const double au_scale = max_radius / sqrt(PLANETS[NEPTUNE].semimajor_axis);
    double distance = hypot(v.x, v.y);
    double scalar = au_scale / sqrt(distance);
    v.x = sun_pos.x + scalar * v.x;
    v.y = sun_pos.y - scalar * v.y; 
    return v;
}

int clamp_day(int month, int day)
{
    int d[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (day > d[month-1]) day = d[month-1];
    return day;
}