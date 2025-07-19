#include <emscripten/emscripten.h>

#include <array>
#include <cmath>
#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

#include "webnoise/noise_generator.hpp"
#include "webnoise/argument.hpp"
#include "webnoise/generator_registry.hpp"

#include "raygui.h"
#include "raylib.h"
#include "raymath.h"

constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int TARGET_FPS = 60;

constexpr unsigned int GUI_HEIGHT = 600;
constexpr unsigned int GUI_WIDTH = 200;

constexpr unsigned int GRAPH_DIMENSIONS = 10.0;

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

  void m_Update();
  void m_Draw3D();
  void m_DrawGUI();

  Camera3D m_Camera;
  bool m_CameraEnabled = true;
};