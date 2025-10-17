// mesh.hpp - manually tessellated shapes with normals & texcoords
#pragma once

// Draw torus: majorSegments around big circle, minorSegments around tube
// R = major radius, r = minor (tube) radius
// Generates normals and texture coordinates
void drawTorus(int majorSegments, int minorSegments, float R, float r);

// Draw cone: slices around base, radius at base, height along Y axis
// Base at Y=0, tip at Y=height. Generates normals and texcoords
void drawCone (int slices, float radius, float height);

// Draw wavy ground: N×N grid, total size×size, wave amplitude amp
// texScale = texture coordinate scale factor. Generates normals and texcoords
void drawWavyGround(int N, float size, float amp, float texScale);
