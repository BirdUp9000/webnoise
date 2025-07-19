#pragma once

#include <vector>

#include "webnoise/argument.hpp"

#include "raylib.h"

struct INoiseGenerator {
  virtual float getNoise(const Vector2 position, const std::vector<CArgument>& args) = 0;
  virtual std::vector<CArgument> getArguments() const = 0;
  virtual ~INoiseGenerator() = default;
};