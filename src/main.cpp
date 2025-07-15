#include "webnoise/noise_generator.hpp"
#include "webnoise/perlin_noise_generator.hpp"
#include "webnoise/webnoise.hpp"

int main() {
  // Creating a generator instance
  // Only Perlin generator for now
  auto gen_ptr = std::make_unique<PerlinNoiseGenerator>();

  // Passing pointer to the generator into app
  CWebNoise app(std::move(gen_ptr));

  // Run
  app.m_Run();

  return EXIT_SUCCESS;
}
