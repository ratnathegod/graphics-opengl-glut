#pragma once
#include "common.hpp"

void hudPrintf(float x, float y, const char* fmt, ...);
void drawNormalLine(float x,float y,float z,float nx,float ny,float nz,float s=0.3f);
void normalFromTriangle(const float A[3], const float B[3], const float C[3]);