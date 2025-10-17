#include "camera.hpp"

static void setProj(const CameraState& c)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (c.mode==0)
        glOrtho(-c.asp*c.dim, c.asp*c.dim, -c.dim, c.dim, -4*c.dim, 4*c.dim);
    else
        gluPerspective(c.fov, c.asp, 0.1, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void applyProjection(const CameraState& cam){ setProj(cam); }

void applyView(const CameraState& cam)
{
    if (cam.mode==2)
    {
        double Ex = 8*cos(rad(cam.th))*cos(rad(cam.ph));
        double Ey = 8*sin(rad(cam.ph));
        double Ez = 8*sin(rad(cam.th))*cos(rad(cam.ph));
        gluLookAt(Ex,Ey,Ez, 0,0,0, 0,1,0);
    }
    else
    {
        glRotatef(cam.ph,1,0,0);
        glRotatef(cam.th,0,1,0);
    }
}