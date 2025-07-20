# WebNoise

WebNoise is a minimal, **easy to contribute** WebAssembly project written in C++ using [raylib](https://www.raylib.com/) and [raygui](https://github.com/raysan5/raygui). It renders procedural 3D noise  directly in the browser using WebGL via Emscripten.

[![Build and deploy](https://github.com/BirdUp9000/webnoise/actions/workflows/deploy.yml/badge.svg)](https://github.com/BirdUp9000/webnoise/actions/workflows/deploy.yml)

**[Live Demo](https://birdup9000.github.io/webnoise/)**

## Features

- Procedural noise generation in C++
- Support for multiple procedural noise generators
- Plugin-like architecture for adding new generators (Static registration pattern)
- GUI with live parameter sliders
- Rendered using raylib + raygui
- Compiled to WebAssembly with Emscripten
- GitHub Actions CI/CD and automatic deployment to GitHub Pages
- Easy to extend and contribute

## Build & Run Locally

To build the project locally using Docker:

```bash
docker build -t webnoise-builder .
docker run --rm -v $(pwd)/out:/out webnoise-builder
```
Run ```python3 -m http.server``` and open ```out/index.html``` in your browser to see the result.

## Contributing
WebNoise is designed to be easy to contribute to.
See CONTRIBUTING.md for instructions on adding new generators, formatting rules, and CI guidelines.

## Structure
```
├── external/               # Git submodules: raylib, raygui
├── include/                # C++ headers
├── src/                    # Source code (.cpp)
├── .github/                # GitHub Actions workflow
├── .clang-format           # Format rules
├── .clang-tidy             # Static analysis
├── .dockerignore
├── .gitignore
├── .gitmodules             # Submodules config
├── CMakeLists.txt          # CMake build script
├── Dockerfile              # Docker build for WebAssembly
├── format.sh               # Formats 'hpp' and 'cpp' files
├── LICENSE
├── CONTRIBUTING.md
├── TODO.md
└── README.md
```

## License

This project is licensed under the [MIT License](LICENSE).
