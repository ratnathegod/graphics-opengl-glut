#pragma once
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

inline double rad(double d){return d*M_PI/180.0;}