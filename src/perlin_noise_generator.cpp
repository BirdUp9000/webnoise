#include "webnoise/perlin_noise_generator.hpp"

float PerlinNoiseGenerator::Perlin(const Vector3 point) const {
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
}

std::vector<Vector3> PerlinNoiseGenerator::Generate(int dim, float scale, float time, int octaves) const {
  std::vector<Vector3> result = {};
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
        float noise = Perlin(Vector3{fx * frequency, fy * frequency, time * frequency});

        total += noise * amplitude;
        maxAmplitude += amplitude;

        amplitude *= 0.5f;
        frequency *= 2.0f;
      }

      float normalized = total / maxAmplitude;

      float xpos = -5.0f + x * spacing;
      float ypos = -5.0f + y * spacing;

      result.emplace_back(Vector3{xpos, normalized, ypos});
    }
  }
  return result;
}