#define RAYGUI_IMPLEMENTATION
#include "webnoise/webnoise.hpp"

static bool camera_enabled = true;

Camera3D CWebNoise::s_camera = {0};
float CWebNoise::time = 0;
float CWebNoise::time_speed = 0;
float CWebNoise::dimensions = 64;
float CWebNoise::scale = 0.1f;
float CWebNoise::height = 2.0f;
float CWebNoise::octaves = 1.0f;

CWebNoise::CWebNoise() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Webnoise");

  // Init camera
  s_camera.position = (Vector3){10.0f, 10.0f, 10.0f};
  s_camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  s_camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  s_camera.fovy = 45.0f;
  s_camera.projection = CAMERA_PERSPECTIVE;

  DisableCursor();
  SetTargetFPS(60);
}

CWebNoise::~CWebNoise() { CloseWindow(); }

void CWebNoise::m_Run() { emscripten_set_main_loop(CWebNoise::m_UpdateDrawFrame, 0, 1); }

void CWebNoise::m_UpdateDrawFrame() {
  CWebNoise::time += time_speed;

  if (!camera_enabled && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse = GetMousePosition();
    if (mouse.x > 320) {
      camera_enabled = true;
    }
  }

  if (camera_enabled) {
    UpdateCamera(&s_camera, CAMERA_FREE);
  }

  if (IsKeyPressed('Z')) {
    s_camera.position = (Vector3){10.0f, 10.0f, 10.0f};
    s_camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  }

  if (IsKeyPressed('F')) {
    camera_enabled = !camera_enabled;
  }

  BeginDrawing();
  ClearBackground(RAYWHITE);

  m_DrawGUI();
  m_Draw3D();

  EndDrawing();
}

void CWebNoise::m_Draw3D() {
  BeginMode3D(s_camera);
  DrawGrid(dimensions, 10.0f / dimensions);
  auto result = m_GenerateNoise(static_cast<unsigned int>(dimensions), scale, time, octaves);

  for (const auto& point : result) {
    float val = point.second;
    Color c = ColorFromHSV(240.0f - val * 240.0f, 0.85f, 0.9f);
    DrawCube(Vector3{point.first.x, point.second * CWebNoise::height, point.first.y}, 0.05f, 0.05f, 0.05f, c);
  }

  EndMode3D();
}

void CWebNoise::m_DrawGUI() {
  DrawFPS(WINDOW_WIDTH - 90, 15);
  DrawRectangle(10, 10, 300, 155, Fade(SKYBLUE, 0.5f));
  DrawRectangleLines(10, 10, 300, 155, BLUE);

  DrawText("Free camera default controls:", 20, 20, 10, BLACK);
  DrawText("- WASD keys are used for directional movement", 40, 40, 10, DARKGRAY);
  DrawText("- SPACE to move up", 40, 60, 10, DARKGRAY);
  DrawText("- CTRL to move down", 40, 80, 10, DARKGRAY);
  DrawText("- Mouse wheel to zoom in / out", 40, 100, 10, DARKGRAY);
  DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
  DrawText("- Press 'F' to prevent 3D Model from rotating", 40, 140, 10, DARKGRAY);

  DrawText("Time speed:", 40, 180, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 200, 220, 20}, "0.01f", "0.25f", &CWebNoise::time_speed, 0.01f, 0.25f);
  DrawText("Dimensions:", 40, 220, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 240, 220, 20}, "64", "256", &CWebNoise::dimensions, 64, 256);
  DrawText("Scale:", 40, 260, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 280, 220, 20}, "0.1f", "0.5f", &CWebNoise::scale, 0.1f, 0.5f);
  DrawText("Height:", 40, 300, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 320, 220, 20}, "2.0f", "10.0f", &CWebNoise::height, 2.0f, 10.0f);
  DrawText("Octaves:", 40, 340, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 360, 220, 20}, "1", "5", &CWebNoise::octaves, 1.0f, 5.0f);
}

std::vector<std::pair<Vector2, float>> CWebNoise::m_GenerateNoise(const unsigned int dim, const float scale,
                                                                  const float time, const float octaves) {
  // Perlin noise computing function
  auto perlin = [](Vector3 point) {
    // Cube coordinates
    int X_coord = static_cast<int>(floor(point.x));
    int Y_coord = static_cast<int>(floor(point.y));
    int Z_coord = static_cast<int>(floor(point.z));

    // Position inside of the cube
    float X_rel = point.x - X_coord;
    float Y_rel = point.y - Y_coord;
    float Z_rel = point.z - Z_coord;

    // Smoothing function fade(t) = 6t⁵ − 15t⁴ + 10t³
    float u = 6 * pow(X_rel, 5) - 15 * pow(X_rel, 4) + 10 * pow(X_rel, 3);
    float v = 6 * pow(Y_rel, 5) - 15 * pow(Y_rel, 4) + 10 * pow(Y_rel, 3);
    float w = 6 * pow(Z_rel, 5) - 15 * pow(Z_rel, 4) + 10 * pow(Z_rel, 3);

    // Computing noise
    float result = 0;
    for (int x = 0; x < 2; x++) {
      for (int y = 0; y < 2; y++) {
        for (int z = 0; z < 2; z++) {
          int X_idx = (X_coord + x) & 255;
          int Y_idx = (Y_coord + y) & 255;
          int Z_idx = (Z_coord + z) & 255;
          int hash = g_PerlinPermutation512[g_PerlinPermutation512[g_PerlinPermutation512[X_idx] + Y_idx] + Z_idx];
          unsigned int index = hash % 12;
          Vector3 gradient = g_PerlinNoiseGradient[index];
          Vector3 offset = {X_rel - x, Y_rel - y, Z_rel - z};

          // Dot product
          float dot = (gradient.x * offset.x + gradient.y * offset.y + gradient.z * offset.z);

          // Weights
          float wx = (x == 0) ? (1.0f - u) : u;
          float wy = (y == 0) ? (1.0f - v) : v;
          float wz = (z == 0) ? (1.0f - w) : w;

          result += dot * wx * wy * wz;
        }
      }
    }
    return 0.5f * (result + 1.0f);
  };

  std::vector<std::pair<Vector2, float>> result = {};
  float spacing = 10.0f / dim;
  for (int y = 0; y < dim; y++) {
    for (int x = 0; x < dim; x++) {
      float fx = x * scale;
      float fy = y * scale;

      float total = 0.0f;
      float amplitude = 1.0f;
      float frequency = 1.0f;
      float maxAmplitude = 0.0f;

      for (int i = 0; i < octaves; i++) {
        float noise = perlin(Vector3{fx * frequency, fy * frequency, time * frequency});

        total += noise * amplitude;
        maxAmplitude += amplitude;

        amplitude *= 0.5f;
        frequency *= 2.0f;
      }

      float normalized = total / maxAmplitude;

      float xpos = -5.0f + x * spacing;
      float ypos = -5.0f + y * spacing;

      result.emplace_back(Vector2{xpos, ypos}, normalized);
    }
  }
  return result;
}