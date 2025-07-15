#!/bin/bash
# Format all C++ source and header files

clang-format -i -style=file \
  $(find src include -type f \( -name '*.cpp' -o -name '*.hpp' \))
