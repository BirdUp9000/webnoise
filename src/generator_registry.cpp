#include "webnoise/generator_registry.hpp"

bool CGeneratorRegistry::add(const std::string& name, FactoryFunction fac) {
  auto map = getFactoryMap();
  if (map.find(name) != map.end()) {
    return false;
  }

  getFactoryMap()[name] = fac;
  return true;
}

std::unique_ptr<INoiseGenerator> CGeneratorRegistry::create(const std::string& name) {
  auto map = getFactoryMap();
  if (map.find(name) == map.end()) {
    return nullptr;
  }

  return map[name]();
}

std::vector<std::string> CGeneratorRegistry::getNames() {
  std::vector<std::string> names;
  for (const auto& [name, _] : getFactoryMap()) {
    names.push_back(name);
  }
  return names;
}