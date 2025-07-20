#pragma once
#include <emscripten/emscripten.h>

#include <array>
#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
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

constexpr std::string DEFAULT_GENERATOR = "Perlin";

constexpr Vector3 DEFAULT_CAMERA_POSITION = {10.0f, 10.0f, 10.0f};
constexpr Vector3 DEFAULT_CAMERA_TARGET = {-3.0f, 0.0f, 0.0f};
constexpr Vector3 DEFAULT_CAMERA_UP = {0.0f, 1.0f, 0.0f};
constexpr float DEFAULT_CAMERA_FOV = 45.0f;
constexpr int DEFAULT_CAMERA_PROJECTION = CAMERA_PERSPECTIVE;

constexpr int FPS = 60;

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

  unsigned int s_CurrentGeneratorIndex;

  void m_Update();
  void m_Draw3D();
  void m_DrawGUI();

  Camera3D m_Camera;
  bool m_CameraEnabled = true;
};