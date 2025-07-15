# WebNoise

WebNoise is a minimal WebAssembly project written in C++ using [raylib](https://www.raylib.com/) and [raygui](https://github.com/raysan5/raygui). It renders procedural 3D noise (Perlin noise) directly in the browser using WebGL via Emscripten.

[![Build and deploy](https://github.com/BirdUp9000/webnoise/actions/workflows/deploy.yml/badge.svg)](https://github.com/BirdUp9000/webnoise/actions/workflows/deploy.yml)

## Demo
> [!NOTE]  
> [Live Demo](https://birdup9000.github.io/webnoise/)


## Features

- Procedural noise generation in C++
- Rendered using raylib + raygui
- Compiled to WebAssembly with Emscripten
- Lightweight and fast
- GitHub Actions CI/CD and automatic deployment to GitHub Pages

## Build & Run Locally

To build the project locally using Docker:

```bash
docker build -t webnoise-builder .
docker run --rm -v $(pwd)/out:/out webnoise-builder
```
Open out/index.html in your browser to see the result.

## Structure
```
├── external/               # Git submodules: raylib, raygui
├── include/                # C++ headers
├── src/                    # Source code (.cpp)
├── .github/                # GitHub Actions workflow
├── .clang-format           # Format rules
├── .dockerignore
├── .gitignore
├── .gitmodules             # Submodules config
├── CMakeLists.txt          # CMake build script
├── Dockerfile              # Docker build for WebAssembly
├── format.sh               # Formats 'hpp' and 'cpp' files
└── README.md
```

## License

This project is licensed under the [MIT License](LICENSE).