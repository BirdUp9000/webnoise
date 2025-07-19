#include "webnoise/argument.hpp"
#include "webnoise/generator_registry.hpp"
#include "webnoise/noise_generator.hpp"

constexpr std::array<int, 256> g_PerlinPermutation256 = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142,
    8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203,
    117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
    71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,
    55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,
    18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250,
    124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189,
    28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,
    129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,
    242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
    181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114,
    67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180};

constexpr std::array<int, 512> makePerlinPermutation512() {
  std::array<int, 512> result{};
  for (int i = 0; i < 256; i++) {
    result[i] = g_PerlinPermutation256[i];
    result[i + 256] = g_PerlinPermutation256[i];
  }
  return result;
}
constexpr std::array<int, 512> g_PerlinPermutation512 = makePerlinPermutation512();

constexpr std::array<Vector3, 12> g_PerlinNoiseGradient = {{{1, 1, 0},
                                                            {-1, 1, 0},
                                                            {1, -1, 0},
                                                            {-1, -1, 0},
                                                            {1, 0, 1},
                                                            {-1, 0, 1},
                                                            {1, 0, -1},
                                                            {-1, 0, -1},
                                                            {0, 1, 1},
                                                            {0, -1, 1},
                                                            {0, 1, -1},
                                                            {0, -1, -1}}};

class PerlinNoiseGenerator final : public INoiseGenerator {
  float perlin(const Vector3 point) const {
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

  float m_Time = 0.0f;

public:
  float getNoise(const Vector2 position, const std::vector<CArgument>& args) override {
    float scale = args[0].m_Value;
    m_Time += args[1].m_Value;
    int octaves = static_cast<int>(args[2].m_Value);

    float x = position.x * scale;
    float y = position.y * scale;

    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; ++i) {
      Vector3 p = {x * frequency, y * frequency, m_Time * frequency};
      float noise = perlin(p);

      total += noise * amplitude;
      maxAmplitude += amplitude;

      amplitude *= 0.5f;
      frequency *= 2.0f;
    }

    return total / maxAmplitude;
  }

  std::vector<CArgument> getArguments() const override {
    return {{"Scale", "0.1f", "0.5f", 0.1f, 0.1f, 0.5f},
            {"Time Speed", "0.01f", "0.25f", 0.01f, 0.01f, 0.25f},
            {"Octaves", "1", "5", 1.0f, 1.0f, 5.0f}};
  }
};

REGISTER_PLUGIN("Perlin", PerlinNoiseGenerator);
