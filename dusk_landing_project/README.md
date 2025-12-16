# Dusk Landing – OpenGL Aerospace Visualization  
Ratnakaru Yalagathala  
CSCI 4229-5229-001 – Computer Graphics  
Fall 2025

---

## Project Overview

**Dusk Landing** is a fixed-function OpenGL simulation of an eight-second reusable booster landing at dusk.  
All geometry—including the booster, tower, pad, grid fins, legs, and cryogenic tank farm—is modeled procedurally in C++ with hierarchical transforms so articulation remains mechanically plausible. The animation is smoothstep-driven, lit with a dusk key/fill setup, and enhanced with planar shadows, fog, textures, and a hybrid flame + particle system tied to engine throttle.

The project demonstrates all core course topics in a single cohesive scene:

- Hierarchical modeling  
- Custom mesh generation  
- Procedural animation  
- Dynamic lighting + materials  
- Fog and planar shadow projection  
- Additive/alpha particle rendering  
- Dual camera system (orbit + cinematic tracker)

The program was developed on macOS but built specifically to compile **warning-free on Linux**, as required by the course.

---

## Build and Run Instructions (Linux first)

### **Using Makefile**
```bash
make clean
make
./dusk_landing
```

### **Direct g++ command (required by assignment)**
```bash
g++ dusk_landing.cpp -o dusk_landing \
    -lGL -lGLU -lglut -lGLEW \
    -std=c++17 -Wall -Wextra -O2
```

### **Required packages on Ubuntu**
```
build-essential
freeglut3-dev
libglew-dev
libglu1-mesa-dev
```

SOIL textures are optional. If the header/library is not found, the program automatically disables texturing and continues with standard lit materials.

### **Platform Notes**
- **Linux:** Uses `<GL/gl.h>`, `<GL/glu.h>`, `<GL/freeglut.h>`, and GLEW when present.  
- **macOS:** Uses system OpenGL/GLUT frameworks. `GL_SILENCE_DEPRECATION` keeps the build log clean.  
- All geometry is procedural; **no external models or shaders** are used.  
- The project builds as a **single C++ translation unit** (`dusk_landing.cpp`), exactly as required.

---

## Controls

- **C** – Toggle orbit/action camera  
- **1 / 2** – Pause / play animation  
- **R** – Reset the eight-second landing  
- **E** – Toggle environment props  
- **P** – Toggle planar shadow  
- **X** – Toggle particle plume + dust  
- **O** – Toggle atmospheric fog  
- **M** – Toggle engine flame mesh  
- **Arrow Keys** – Orbit camera yaw/pitch  
- **Mouse wheel / + / -** – Orbit camera zoom  
- **ESC** – Quit  

A controls summary is shown inside the on-screen HUD.

---

## Features to Notice (for Grading)

### **1. Procedural Booster Model**
- Hierarchical animation of legs, grid fins, gimbaling engine bell, and throttle-responsive flame.  
- Smoothstep timing ensures natural motion across all phases of descent.

### **2. Environment Geometry (All Procedural)**
- Multi-level truss tower with cross-bracing, lights, and railings.  
- Cryogenic tank farm with banded texture wraps and platforms.  
- Landing pad with concentric rings, scorch decals, and a shallow flame trench.

### **3. Particle System**
- Additive hot-exhaust particles + alpha-blended dust near ground contact.  
- Responds to gimbal direction, altitude, and throttle.  
- Clamped above ground to avoid flicker.

### **4. Camera System**
- **Orbit camera** for grading and scene inspection.  
- **Action camera** smoothly tracks the booster, banks slightly with engine gimbal, and eases into the landing shot.

### **5. Rendering & Effects**
- Dusk lighting with warm/cool contrast.  
- Atmospheric fog to push depth.  
- Planar shadow projection matching booster position.  
- Textured pad + tank bands, with clean fallback if textures are unavailable.

### **6. Clean Compilation Requirement**
- Builds **warning-free** on macOS and Linux (`-std=c++17 -Wall -Wextra`).  
- Platform-specific headers chosen via compile-time guards.  
- All deprecated calls are silenced or avoided per assignment rubric.

---

## Tested Environments

- **macOS 14.4**, Apple Clang — clean build, 0 warnings  
- **Ubuntu 22.04**, g++11/GLEW/GLUT stack — clean build, 0 warnings  

---

## Additional Notes

- All geometry, animation, and rendering logic are implemented manually without GLU/GLUT shapes or external models.  
- Textures are optional; the project gracefully degrades to pure lighting if SOIL is absent.  
- The single-file structure is intentional to match the course requirement and simplify Linux compilation.

---

## Author  
**Ratnakaru Yalagathala**  
CSCI 4229-5229-001 – Computer Graphics  
Fall 2025