#define RAYGUI_IMPLEMENTATION
#include "webnoise/webnoise.hpp"

CWebNoise::CWebNoise(std::unique_ptr<INoiseGenerator> generator) : p_NoiseGenerator(std::move(generator)) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Webnoise");

  // Init camera
  m_Camera = {0};
  m_Camera.position = (Vector3){10.0f, 10.0f, 10.0f};
  m_Camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  m_Camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  m_Camera.fovy = 45.0f;
  m_Camera.projection = CAMERA_PERSPECTIVE;

  DisableCursor();
  SetTargetFPS(60);
}

CWebNoise::~CWebNoise() { CloseWindow(); }

void CWebNoise::m_Run() {
  emscripten_set_main_loop_arg([](void* arg) { static_cast<CWebNoise*>(arg)->m_Update(); }, this, 0, 1);
}

void CWebNoise::m_Update() {
  m_Time += m_TimeSpeed;

  if (!m_CameraEnabled && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse = GetMousePosition();
    if (mouse.x > 320) {
      m_CameraEnabled = true;
    }
  }

  if (m_CameraEnabled) {
    UpdateCamera(&m_Camera, CAMERA_FREE);
  }

  if (IsKeyPressed('Z')) {
    m_Camera.position = (Vector3){10.0f, 10.0f, 10.0f};
    m_Camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  }

  if (IsKeyPressed('F')) {
    m_CameraEnabled = !m_CameraEnabled;
  }

  BeginDrawing();
  ClearBackground(RAYWHITE);

  m_DrawGUI();
  m_Draw3D();

  EndDrawing();
}

void CWebNoise::m_Draw3D() {
  BeginMode3D(m_Camera);
  DrawGrid(m_Dimensions, 10.0f / m_Dimensions);
  auto result = p_NoiseGenerator->Generate(static_cast<unsigned int>(m_Dimensions), m_Scale, m_Time, m_Octaves);

  for (const auto& element : result) {
    Color c = ColorFromHSV(240.0f - element.y * 240.0f, 0.85f, 0.9f);
    DrawCube(Vector3{element.x, element.y * m_Height, element.z}, 0.05f, 0.05f, 0.05f, c);
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
  GuiSlider(Rectangle{40, 200, 220, 20}, "0.01f", "0.25f", &m_TimeSpeed, 0.01f, 0.25f);
  DrawText("Dimensions:", 40, 220, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 240, 220, 20}, "64", "256", &m_Dimensions, 64, 256);
  DrawText("Scale:", 40, 260, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 280, 220, 20}, "0.1f", "0.5f", &m_Scale, 0.1f, 0.5f);
  DrawText("Height:", 40, 300, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 320, 220, 20}, "2.0f", "10.0f", &m_Height, 2.0f, 10.0f);
  DrawText("Octaves:", 40, 340, 15, DARKGRAY);
  GuiSlider(Rectangle{40, 360, 220, 20}, "1", "5", &m_Octaves, 1.0f, 5.0f);
}