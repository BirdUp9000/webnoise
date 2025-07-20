#include "webnoise/argument.hpp"
#include "webnoise/generator_registry.hpp"
#include "webnoise/noise_generator.hpp"

// =========================================================
// This is an example noise generator for the WebNoise project.
//
// You can use this file as a reference for implementing your own
// generator. To register a new generator:
//
// 1. Inherit from `INoiseGenerator`
// 2. Override `getNoise()` and `getArguments()`
// 3. Use the macro REGISTER_PLUGIN(name, class) at the end
//
// The generator will automatically appear in the GUI dropdown,
// and your parameters (arguments) will be rendered as sliders.
//
// Each time getNoise() is called, you must return a flat list of
// 3D points (Vector3) representing the noise surface.
// =========================================================

class DummyNoiseGenerator final : public INoiseGenerator {
public:
  // This function generates the 3D points for rendering the noise.
  // - `dimensions` is the number of cells along X and Y (e.g. 10, 50, etc.)
  // - `args` contains values of user-specified arguments (sliders)
  //
  // The return value must be a std::vector<Vector3>,
  // where each Vector3 is {x, y, z} in world space.
  std::vector<Vector3> getNoise(const float dimensions, const std::vector<CArgument>& args) override {
    std::vector<Vector3> result = {};

    for (int y = 0; y <= static_cast<int>(dimensions); y++) {
      for (int x = 0; x <= static_cast<int>(dimensions); x++) {
        // This dummy generator just places all points at height = 1.0f
        // Height must be between [0, 1]
        result.emplace_back(Vector3{static_cast<float>(x), 1.0f, static_cast<float>(y)});
      }
    }
    return result;
  }

  // This function defines which parameters the generator exposes to the user.
  // Each parameter is represented by a CArgument:
  //   name, leftLabel, rightLabel, defaultValue, minValue, maxValue
  //
  // The GUI automatically renders each CArgument as a horizontal slider.
  // Example:
  //   return {
  //     {"Scale", "Low", "High", 0.1f, 0.01f, 1.0f},
  //     {"Octaves", "1", "5", 3.0f, 1.0f, 5.0f}
  //   };
  //
  // This dummy generator doesn't use any parameters.
  std::vector<CArgument> getArguments() const override { return {}; }
};

// This macro registers the generator in the global plugin registry.
// The first argument is the name shown in the dropdown menu in GUI.
// The second is the generator class itself.
//
// IMPORTANT: This must be at global scope and not inside a namespace.
REGISTER_PLUGIN("Dummy", DummyNoiseGenerator);
