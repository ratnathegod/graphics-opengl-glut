#include "lighting.hpp"

void setupLighting(const LightState& L)
{
    if (!L.enabled){ glDisable(GL_LIGHTING); return; }
    glEnable(GL_NORMALIZE); glEnable(GL_LIGHTING); glShadeModel(GL_SMOOTH);

    double Ex = L.R*cos(rad(L.theta))*cos(rad(L.elev));
    double Ey = L.R*sin(rad(L.elev));
    double Ez = L.R*sin(rad(L.theta))*cos(rad(L.elev));
    float pos[4] = {(float)Ex,(float)Ey,(float)Ez,1.0f};

    float Ld[4]={0.9f,0.9f,0.9f,1}, La[4]={0.1f,0.1f,0.15f,1}, Ls[4]={1,1,1,1};
    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,Ld);
    glLightfv(GL_LIGHT0,GL_AMBIENT,La);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Ls);
    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,L.amb);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,L.diff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,L.spec);
    glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,L.shiny);

    // marker
    glDisable(GL_LIGHTING);
    glPointSize(8); glBegin(GL_POINTS); glColor3f(1,1,0); glVertex3f(pos[0],pos[1],pos[2]); glEnd();
    glEnable(GL_LIGHTING);
}

void tickLight(LightState& L, float dt)
{
    if (L.animate){ L.theta += 30.0*dt; if (L.theta>360.0) L.theta-=360.0; }
}