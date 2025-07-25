cmake_minimum_required(VERSION 3.25)
project(webnoise LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding Raylib
add_subdirectory(external/raylib)

set(SOURCES
    src/main.cpp
    src/webnoise.cpp
    src/generator_registry.cpp
)

# All .cpp files from src/generators/
file(GLOB GENERATOR_SOURCES src/generators/*.cpp)

add_executable(webnoise ${SOURCES} ${GENERATOR_SOURCES})

target_link_libraries(webnoise raylib)

target_include_directories(webnoise PRIVATE
    include
    external/raygui/src
)

option(BUILD_SHARED_LIBS OFF)
option(BUILD_EXAMPLES OFF)

# Emscripten flags
if(EMSCRIPTEN)
    add_definitions(-DPLATFORM_WEB -DNDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -flto")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -flto")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=0 \
    -s ERROR_ON_UNDEFINED_SYMBOLS=1")
endif()

