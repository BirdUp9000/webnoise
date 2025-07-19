#define RAYGUI_IMPLEMENTATION
#include "webnoise/webnoise.hpp"

CWebNoise::CWebNoise() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Webnoise");

  // Get all registered generator's names
  m_GenRegistered = CGeneratorRegistry::getNames();
  if (m_GenRegistered.size() == 0) {
    throw std::runtime_error("No generator provided");
  }

  // Perlin generator should be first
  auto it = std::find(m_GenRegistered.begin(), m_GenRegistered.end(), "Perlin");
  if (it != m_GenRegistered.end()) {
    std::rotate(m_GenRegistered.begin(), it, it + 1);
  }

  // Initialize m_GenState structure
  m_GenState.m_Name = "Perlin";
  m_GenState.m_Gen = CGeneratorRegistry::create(m_GenState.m_Name);
  m_GenState.m_Args = m_GenState.m_Gen->getArguments();

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
  // m_Time += m_TimeSpeed;

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
  DrawGrid(GRAPH_DIMENSIONS, 10.0f / GRAPH_DIMENSIONS);
  float offset = GRAPH_DIMENSIONS / 2.0f;

  for (int x = 0; x <= GRAPH_DIMENSIONS; x++) {
    for (int y = 0; y <= GRAPH_DIMENSIONS; y++) {
      float fx = static_cast<float>(x) - offset;
      float fy = static_cast<float>(y) - offset;

      const float result = m_GenState.m_Gen->getNoise(Vector2{fx, fy}, m_GenState.m_Args);
      Color c = ColorFromHSV(240.0f - result * 240.0f, 0.85f, 0.9f);

      DrawCube(Vector3{fx, result, fy}, 0.05f, 0.05f, 0.05f, c);
    }
  }

  EndMode3D();
}

void CWebNoise::m_DrawGUI() {
  static int s_CurrentGenerator = 0;

  // Draw FPS counter and controls
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

  // Draw Generator's control GUI
  const bool left = GuiButton(Rectangle{10, 180, 30, 30}, " < ");
  const bool right = GuiButton(Rectangle{280, 180, 30, 30}, " > ");

  if (left || right) {
    if (left && s_CurrentGenerator == 0)
      s_CurrentGenerator = m_GenRegistered.size() - 1;
    else if (right && m_GenRegistered.size() - 1 == s_CurrentGenerator)
      s_CurrentGenerator = 0;
    else if (left && s_CurrentGenerator > 0)
      --s_CurrentGenerator;
    else if (right && m_GenRegistered.size() - 1 > s_CurrentGenerator)
      ++s_CurrentGenerator;

    m_GenState.m_Name = m_GenRegistered.at(s_CurrentGenerator);
    m_GenState.m_Gen = CGeneratorRegistry::create(m_GenState.m_Name);
    m_GenState.m_Args = m_GenState.m_Gen->getArguments();
  }

  // Current generator's name
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  Rectangle generatorNameBounds = {50, 180, 220, 30};

  int textWidth = GetTextWidth(m_GenState.m_Name.c_str());
  int textHeight = GuiGetStyle(DEFAULT, TEXT_SIZE);

  Vector2 textPos = {generatorNameBounds.x + (generatorNameBounds.width - textWidth) / 2.0f,
                     generatorNameBounds.y + (generatorNameBounds.height - textHeight) / 2.0f};

  GuiDrawText(m_GenState.m_Name.c_str(),
              {textPos.x, textPos.y, static_cast<float>(textWidth), static_cast<float>(textHeight)}, TEXT_ALIGN_LEFT,
              DARKGRAY);

  // Draw controls
  GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
  float xpos = 50;
  float ypos = 220;
  for (auto& argument : m_GenState.m_Args) {
    DrawText(argument.m_Name.c_str(), xpos, ypos, 15, DARKGRAY);
    GuiSlider(Rectangle{xpos, ypos + 20.0f, 220, 20}, argument.m_TextLeft.c_str(), argument.m_TextRight.c_str(),
              &argument.m_Value, argument.m_MinValue, argument.m_MaxValue);
    ypos += 50;
  }
}