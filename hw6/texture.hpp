// texture.hpp - power-of-two procedural textures
#pragma once
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// Generate checker pattern texture: size×size pixels, check pixel squares
GLuint makeCheckerTexture(int size, int check);

// Generate stripe pattern texture: size×size pixels, period pixel stripes
GLuint makeStripeTexture (int size, int period);
