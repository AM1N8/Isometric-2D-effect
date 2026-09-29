#include "raylib.h"
#include <raymath.h>
#include <math.h>

int main() {
    const int screen_width = 1000;
    const int screen_height = 1000;
    const float scale = 3.0f;
    InitWindow(screen_width, screen_height, "Demo");
    SetTargetFPS(144);

    // Load block texture
    Image image = LoadImage("assets/isoblock.png");
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    const int mapWidth = 20;
    const int mapHeight = 20;

    // Map size in pixels 
    float mapPixelWidth  = (mapWidth + mapHeight) * (texture.width * scale / 2);
    float mapPixelHeight = (mapWidth + mapHeight) * (texture.height * scale / 4);
    float offsetX = (screen_width  - mapPixelWidth) / 2.0f;
    float offsetY = (screen_height - mapPixelHeight) / 2.0f;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        // Convert mouse pos to isometric grid coords
        float mx = mouse.x - (offsetX + mapPixelWidth / 2);
        float my = mouse.y - offsetY;

        // Reverse iso transform
        float gridX = (mx / (texture.width * scale / 2) + my / (texture.height * scale / 4)) / 2.0f;
        float gridY = (my / (texture.height * scale / 4) - mx / (texture.width * scale / 2)) / 2.0f;

        BeginDrawing();
        ClearBackground(SKYBLUE);

        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                float isoX = (x - y) * (texture.width * scale / 2);
                float isoY = (x + y) * (texture.height * scale / 4);

                // Calculate dome height based on distance from hovered tile
                float dist = Vector2Distance((Vector2){x, y}, (Vector2){gridX, gridY});
                float heightOffset = 0;

                float radius = 5.0f;  
                if (dist < radius) {
                    // Dome falloff
                    float t = (radius - dist) / radius;
                    heightOffset = t * 100.0f; // 50 px max raise
                }

                DrawTextureEx(texture, (Vector2){isoX + offsetX + mapPixelWidth / 2, isoY + offsetY - heightOffset}, 0.0f, scale, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
