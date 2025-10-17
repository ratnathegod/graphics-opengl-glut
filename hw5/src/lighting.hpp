#pragma once
#include "common.hpp"

struct LightState {
    int enabled=1;
    int animate=1;
    double R=8.0; double theta=0.0; double elev=25.0;
    float shiny=64.0f;
    float spec[4] = {1,1,1,1};
    float diff[4] = {1,1,1,1};
    float amb[4]  = {0.2f,0.2f,0.2f,1};
};

void setupLighting(const LightState& L);
void tickLight(LightState& L, float dt);