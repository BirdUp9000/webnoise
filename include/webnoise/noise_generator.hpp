#pragma once

#include <vector>

#include "raylib.h"

struct INoiseGenerator {
public:
  virtual std::vector<Vector3> Generate(int dim, float scale, float time, int octaves) const = 0;
  virtual ~INoiseGenerator() = default;
};