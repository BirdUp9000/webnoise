#pragma once

#include <vector>

#include "raylib.h"
#include "webnoise/argument.hpp"

struct INoiseGenerator {
  virtual std::vector<Vector3> getNoise(const float dimensions, const std::vector<CArgument>& args) = 0;
  virtual std::vector<CArgument> getArguments() const = 0;
  virtual ~INoiseGenerator() = default;
};