# TODO for WebNoise

This file contains open tasks and feature ideas. Contributions are welcome!

---

## 🔧 Core Functionality

- [ ] Support 2D noise rendering mode (top-down heightmap)
- [ ] Add mesh-based terrain rendering as alternative to cubes
- [ ] Improve performance for large dimensions (e.g. 100×100)

---

## 🧪 Generators

- [ ] Add Simplex Noise generator
- [ ] Add Cellular/Worley Noise generator
- [ ] Add Value Noise
- [ ] Add Fractal Brownian Motion (FBM)
- [ ] Add random terrain generator with seed control
- [ ] Allow hybrid generators (e.g. combine two types of noise)

---

## 🧱 UI/UX

- [ ] Add reset-to-defaults button for generator arguments
- [ ] Show exact numeric value for each slider
- [ ] Add light/dark theme toggle

---

## 📦 DevOps & CI

- [ ] Add ```.clang-tidy```
- [ ] Add ```cppcheck```
- [ ] Add ```SonarQube```
- [ ] Create our own Docker Image 
- [ ] Add unit tests for generators
- [ ] Lint Dockerfile and GitHub Actions with CI

---

## 📚 Documentation

- [ ] **Generate Doxygen docs for all interfaces and classes**
- [ ] Add architecture diagram (e.g. registry, GUI, render loop)
- [ ] Add example commit walkthrough (step-by-step contribution example)
- [ ] Explain argument format in more detail (types, units)

---

## 🧹 Code Quality

- [ ] Add comments to `CArgument` fields for clarity
- [ ] Extract magic constants (e.g. 10.0f, -5.0f) into ```webnoise.hpp```

---

## 🏗️ Potential Features

- [ ] Add camera bookmarks / presets
- [ ] Export current view as PNG or WebP

---
Feel free to open an issue or PR for anything listed above — or propose something new!