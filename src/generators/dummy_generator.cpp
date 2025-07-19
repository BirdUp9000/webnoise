#include "webnoise/argument.hpp"
#include "webnoise/generator_registry.hpp"
#include "webnoise/noise_generator.hpp"

class DummyNoiseGenerator final : public INoiseGenerator {
public:
  float getNoise(const Vector2 position, const std::vector<CArgument>& args) override { return 1.0f; }

  std::vector<CArgument> getArguments() const override { return {}; }
};

REGISTER_PLUGIN("Dummy", DummyNoiseGenerator);