#include "raylib.h"
#include <math.h>

typedef struct {
    float distance;
    float angle;
    float speed;
    float radius;
    Color colour;
} Planet;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "orbits");

    Vector2 sun_center = { (float)screenWidth / 2.0f, (float)screenHeight / 2.0f };

    SetTargetFPS(60);

    Planet earth = {100.0f, 0.0f, 0.015f, 10.0f, SKYBLUE};
    earth.angle += earth.speed;
    
    if (earth.angle > 2.0f * PI) {
        earth.angle -= 2.0f * PI;
    }


    while (!WindowShouldClose())
    {
        earth.angle += earth.speed;
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawCircleV(sun_center, 40, YELLOW);

            Vector2 earth_pos;
            earth_pos.x = sun_center.x + earth.distance * cosf(earth.angle);
            earth_pos.y = sun_center.y + earth.distance * sinf(earth.angle);
            DrawCircleLinesV(sun_center, earth.distance, GRAY);
            DrawCircleV(earth_pos, earth.radius, earth.colour);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}