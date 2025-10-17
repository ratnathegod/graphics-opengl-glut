# Lighting + Textures (GLU-free objects)

A small OpenGL program that renders a 3D scene of **textured solid objects** under user control.  
- **No GLU/GLUT objects** used for geometry (torus, cone, wavy terrain are manually tessellated).  
- **No GLU matrix helpers** (`gluLookAt`, `gluPerspective`) — we implement our own.  
- Textures are **power-of-two** (256×256) checker/stripes generated procedurally.  
- Dynamic point light orbits the scene; lighting uses fixed-function pipeline.

## Build
```bash
# macOS (Xcode CLT + GLUT framework)
make

# Linux (install freeglut): sudo apt install freeglut3-dev
make


Run:

./textured_lighting
