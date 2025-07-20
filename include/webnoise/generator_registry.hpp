#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "noise_generator.hpp"

#define REGISTER_PLUGIN(generatorName, noiseGeneratorClass) \
  static bool generatorClass##_noiseGeneratorClass =        \
      CGeneratorRegistry::add(generatorName, []() { return std::make_unique<noiseGeneratorClass>(); })

/**
 * @brief https://dxuuu.xyz/cpp-static-registration.html
 */
class CGeneratorRegistry {
public:
  using FactoryFunction = std::function<std::unique_ptr<INoiseGenerator>()>;
  static bool add(const std::string& name, FactoryFunction fac);
  static std::unique_ptr<INoiseGenerator> create(const std::string& name);
  static std::vector<std::string> getNames();

private:
  using FactoryMap = std::map<std::string, FactoryFunction>;
  // Use Meyer's singleton to prevent SIOF
  static FactoryMap& getFactoryMap() {
    static FactoryMap map;
    return map;
  }
};