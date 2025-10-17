#pragma once
#include "common.hpp"

extern int gShowNormals; // toggled by keyboard

void drawGrid(int n, double s);
void drawTorus(double R,double r,int nu,int nv);
void drawHelicoid(double umax,double c,int nu,int nv);
void drawRock(double s);