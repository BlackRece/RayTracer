# RayTracer 🎨

[![Windows Build](https://img.shields.io/badge/Windows-Supported-0078d7)](https://github.com/BlackRece/RayTracer)
[![Linux Build](https://img.shields.io/badge/Linux-Supported-1793d1)](https://github.com/BlackRece/RayTracer_Linux)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

A cross-platform C++ raytracer with custom memory management, developed for a Computer Games Programming degree assignment.

![Raytracer Demo](screenshots/demo.png) <!-- Add your screenshot path -->

## Features ✨
- **3 Rendering Modes**:
    - `BasicRender`: Simple sphere rendering
    - `SimpleShrinking`: Optimized rendering with thread support (`--thread`)
    - `SmoothScaling`: Dynamic scene generation (default)
- **Memory Management**:
    - Custom memory pooling system
    - Runtime configuration with `MemoryManager::enablePooling()`
- **Cross-Platform**:
    - Windows (MSVC) and Linux (GCC) support
    - Platform-specific memory optimizations
- **Configuration**:
    - JSON scene/settings loading (`--json settings.json`)
    - Command-line arguments for runtime control
- **Performance Metrics**:
    - High-resolution timing with `<chrono>`
    - Thread utilization reporting

## Installation 🛠️

### Dependencies
- C++17 compiler (MSVC or GCC)
- CMake 3.12+
- [nlohmann/json](https://github.com/nlohmann/json) (included)

### Build Instructions
```bash
# Clone repository (Windows/Linux versions separate)
git clone https://github.com/BlackRece/RayTracer.git  # Windows
git clone https://github.com/BlackRece/RayTracer_Linux.git  # Linux

# Build with CMake
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```
## Usage 🚀
```bash
# Basic render
./RayTracer --basic

# Multi-threaded SimpleShrinking
./RayTracer --simple --thread

# SmoothScaling with 200 spheres
./RayTracer --smooth --json custom_settings.json
```

### Command Line Options
| **Option** | **Description**               |
|------------|-------------------------------|
| `--json`   | Load settings from JSON file  |
| `--basic`  | Basic rendering mode          |
| `--simple` | Optimized shrinking algorithm |
| `--smooth` | Smooth scaling (default)      |
| `--thread` | Enable multi-threading        |

## Project Structure 📂

```
RayTracer/
├── src/
│   ├── RayTracer/       # Core rendering logic
│   │   ├── Tracer.cpp   # Main rendering pipeline
│   │   └── JsonLoader   # Settings serialization
│   └── MemoryManager/   # Custom allocator system
├── resources/           # Example scenes/settings
├── docs/                # Assignment documentation
└── CMakeLists.txt       # Cross-platform build config
```

## Technical Highlights 💻
- **Memory Manager:**
  - Pool-based allocation strategies
  - Fragmentation prevention through block sizing
- **Rendering Pipeline:**
  - Sphere-based scene generation
  - Multithreaded ray intersection testing
  - Recursive lighting calculations
- **Cross-Platform Support:**
  - Windows: WSL-compatible memory subsystem
  - Linux: POSIX-compliant threading model

## License 📜

This project is licensed under the **GNU General Public License v3.0** - see the [LICENSE](LICENSE) file for full details.

```text
Copyright (C) 2023 BlackRece
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions. See the LICENSE file for details.
```

**Key requirements**:
- Any derivative work must remain open-source under GPLv3
- Modifications must be clearly documented
- Commercial use requires original author's permission

*The license header in source files matches the [original work's GPLv3 notice](https://www.scratchapixel.com) from Scratchapixel.*

## 🎓 Assignment Context

Developed for the **Computer Games Programming** degree module at [University Name], this project demonstrates:

### Key Requirements Met
✅ **Custom Memory Manager**
- Implemented pooling allocator with platform-specific optimizations
- Windows/Linux memory subsystem integration

✅ **Cross-Platform Compatibility**
- Dual repository structure ([Windows](https://github.com/BlackRece/RayTracer) | [Linux](https://github.com/BlackRece/RayTracer_Linux))
- CMake build system supporting MSVC and GCC

✅ **Core Raytracing Features**
- Multiple rendering algorithms (Basic/Simple/Smooth)
- Multithreading implementation
- JSON serialization system

### Academic Focus
| Aspect              | Implementation Details                 |
|---------------------|----------------------------------------|
| **Optimization**    | Memory pooling, thread-local allocation|
| **Architecture**    | Platform-abstracted memory subsystem   |
| **Analysis**        | Runtime metrics via `<chrono>`         |

*Created as part of my games programming portfolio to showcase low-level systems programming and 3D rendering fundamentals.*

## Future Improvements 🔮
- GPU acceleration with CUDA/OpenCL
- BVH acceleration structure
- Additional primitive types (meshes, cubes)
- Interactive preview window