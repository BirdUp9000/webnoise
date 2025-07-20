# Contributing to WebNoise

Thank you for your interest in contributing to WebNoise!

This project is designed to be easily extendable. The most common contribution is adding new **generators**. This guide explains how the system works and how to make clean, self-contained contributions.

---

## Writing a Generator

Generators are C++ classes that inherit from `INoiseGenerator`. They must implement two methods:

```cpp
std::vector<Vector3> getNoise(float dimensions, const std::vector<CArgument>& args) override;
std::vector<CArgument> getArguments() const override;
```

**[Example](/src/generators/dummy.cpp)**

##### How Argument System Works
Each generator can expose a list of parameters (sliders in the GUI). These are returned from getArguments():
```cpp
return {
  {"Scale", "0.1f", "0.5f", 0.1f, 0.1f, 0.5f}, // name, min label, max label, default, min, max
  {"Octaves", "1", "5", 3.0f, 1.0f, 5.0f}
};
```
The user can adjust them at runtime. Values are passed into getNoise() as args[i].m_Value.

##### Registering the Generator
You must use REGISTER_PLUGIN("Name", ClassName) to register your generator. This enables dynamic selection from the GUI.
```cpp
REGISTER_PLUGIN("MyNoise", MyNoiseGenerator);
```

---

## Making other improvements
- You're welcome to contribute in other areas too:
- Documentation
- Code cleanup or refactoring
- DevOps improvements (Docker, CI/CD, GitHub Actions)

Improving the architecture or modularity
If you’re not adding a generator but still want to help — check check [TODO.md](/TODO.md) for open tasks.
Please annotate your pull requests with a short explanation of what was changed and why.

---

## Before You Submit
- Run the code formatter: ./format.sh
- Make sure your generator is isolated and does not break others
- Confirm that the app builds and runs in Docker
- Use clear commit messages

Thank you again for contributing!