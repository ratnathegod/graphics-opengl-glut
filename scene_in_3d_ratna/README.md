# Procedural 3D Scene — Instancing & View Control

This OpenGL/GLUT program creates a simple 3D scene that can be viewed from different eye positions.  
The scene includes:
- A procedurally generated twisted torus (unique object)
- A generic superellipsoid used multiple times with different sizes, rotations, positions, and colors

Hidden surfaces are removed using depth testing and back-face culling.

---

## Build

### macOS
```bash
make
Linux / Ubuntu
bash
Copy code
sudo apt-get install -y build-essential freeglut3-dev
# Edit Makefile to set LIBS = -lglut -lGLU -lGL -lm
make
Run
bash
Copy code
./scene
Controls
Arrow keys → rotate view

PageUp / PageDown → zoom

p → toggle orthographic / perspective

r → reset camera

Esc → quit

Time Spent
~2 hours