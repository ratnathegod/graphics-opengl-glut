# HW4 — Ratnakaru Yalagathala — OpenGL Projections & First-Person (No GLUT/GLU)

This program demonstrates:

- Orthographic overhead view
- Perspective overhead view
- First-person navigation (perspective)

Toggle modes with `m`.

## Build

Dependencies: OpenGL 3.3+, GLFW 3.x, GLM, C++17

macOS (Homebrew):

```bash
brew install glfw glm
make
./hw4
```

Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y build-essential libglfw3-dev libglm-dev mesa-utils
make
./hw4
```

## Run

From the project directory:

```bash
./hw4
```

Or use the Makefile convenience target:

```bash
make run
```

## Controls

- m: cycle modes
- Esc: quit
- R: reset camera

Overhead (Ortho/Persp):

- Arrow keys: orbit (Left/Right = azimuth, Up/Down = elevation)
- [ / ]: zoom

First-Person:

- W/S: forward/backward
- A/D: strafe
- Arrow keys: yaw/pitch
- Shift: faster

## Troubleshooting

- Compiler error: `GLFW/glfw3.h: file not found`
	- Install dependencies first: `brew install glfw glm` (macOS) or the apt packages on Linux.
- Runtime error: cannot load `libglfw.3.dylib`
	- The Makefile sets rpath for `/opt/homebrew/lib` and `/usr/local/lib`. If Homebrew is installed in a custom prefix, add it to `LDFLAGS` in the Makefile (both `-L` and `-Wl,-rpath,`), or export `DYLD_LIBRARY_PATH` accordingly.
- Window not visible
	- On macOS the window may open behind other apps—check the Dock or minimize other windows.
