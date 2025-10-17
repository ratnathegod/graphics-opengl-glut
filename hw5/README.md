# HW5 — Lighting with Custom Normals (Modular)

## Build
### macOS
```bash
make
./hw5
```

### Linux

```bash
sudo apt-get install -y freeglut3-dev mesa-common-dev libgl1-mesa-dev
make
./hw5
```

## Controls

* **Arrow Keys**: Rotate view (azimuth/elevation)
* **m**: Cycle projections (0 Ortho, 1 Persp, 2 FPV)
* **- / +**: Field of View
* **l**: Toggle lighting
* **Space**: Toggle light animation
* **[ / ]**: Light azimuth
* **; / \**: Light elevation
* **, / .**: Light radius
* **n**: Toggle normals debug
* **0**: Reset view
* **q / Esc**: Quit

## Normals & Geometry

* **Torus**: analytic per-vertex normals.
* **Helicoid**: `ru × rv` normalized.
* **Rock**: face normals per triangle.

## Time Spent 5 hours

## to run on macos cd /Users/ratna/Desktop/hw5
## make
## ./hw5