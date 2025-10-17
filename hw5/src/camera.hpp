#pragma once
#include "common.hpp"

struct CameraState {
    int th=30, ph=15;   // angles
    int mode=1;         // 0=ortho,1=persp,2=fpv
    int fov=60; double asp=1.0; double dim=8.0; 
};

void applyProjection(const CameraState& cam);
void applyView(const CameraState& cam);