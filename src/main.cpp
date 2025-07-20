#include "webnoise/webnoise.hpp"

int main() {
  CWebNoise app;
  try {
    app.m_Run();
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
