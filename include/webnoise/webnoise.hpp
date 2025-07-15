#include <emscripten/emscripten.h>

#include <array>
#include <cmath>
#include <memory>
#include <vector>
#include <webnoise/noise_generator.hpp>

#include "raygui.h"
#include "raylib.h"
#include "raymath.h"

constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int TARGET_FPS = 60;

constexpr unsigned int GUI_HEIGHT = 600;
constexpr unsigned int GUI_WIDTH = 200;

class CWebNoise final {
public:
  CWebNoise(std::unique_ptr<INoiseGenerator> generator);
  ~CWebNoise();
  void m_Run();

private:
  void m_Update();
  void m_Draw3D();
  void m_DrawGUI();

  std::unique_ptr<INoiseGenerator> p_NoiseGenerator;

  Camera3D m_Camera;
  bool m_CameraEnabled = true;

  float m_Time = 0;
  float m_TimeSpeed = 0.01f;
  float m_Dimensions = 64;
  float m_Scale = 0.1f;
  float m_Height = 2.0f;
  float m_Octaves = 1.0f;
};