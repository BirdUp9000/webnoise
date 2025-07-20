#include <emscripten/emscripten.h>

#include <array>
#include <cmath>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "webnoise/argument.hpp"
#include "webnoise/generator_registry.hpp"
#include "webnoise/noise_generator.hpp"

constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr unsigned int WINDOW_WIDTH = 1200;
constexpr unsigned int TARGET_FPS = 60;

class CWebNoise final {
public:
  CWebNoise();
  ~CWebNoise();
  void m_Run();

private:
  struct CGeneratorState {
    std::string m_Name;
    std::shared_ptr<INoiseGenerator> m_Gen;
    std::vector<CArgument> m_Args;
  };

  CGeneratorState m_GenState;
  std::vector<std::string> m_GenRegistered;

  float m_Dimensions = 30.0f;
  float m_Height = 3.0f;

  void m_Update();
  void m_Draw3D();
  void m_DrawGUI();

  Camera3D m_Camera;
  bool m_CameraEnabled = true;
};