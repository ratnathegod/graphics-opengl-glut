// mesh.cpp - manual tessellation of basic 3D shapes
#include <cmath>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "mesh.hpp"
#ifndef PI
#define PI 3.14159265358979323846
#endif

void drawTorus(int M, int N, float R, float r)
{
    // param: u in [0,2pi) along major circle, v in [0,2pi) minor
    for (int i=0;i<M;++i)
    {
        float u0 = (float)i/M*2*PI;
        float u1 = (float)(i+1)/M*2*PI;
        glBegin(GL_QUAD_STRIP);
        for (int j=0;j<=N;++j)
        {
            float v = (float)j/N*2*PI;

            float cu0=cos(u0), su0=sin(u0), cv=cos(v), sv=sin(v);
            float cu1=cos(u1), su1=sin(u1);

            // ring 0
            float x0 = (R + r*cv)*cu0;
            float y0 =  r*sv;
            float z0 = (R + r*cv)*su0;
            float nx0 = cv*cu0;
            float ny0 = sv;
            float nz0 = cv*su0;
            glNormal3f(nx0, ny0, nz0);
            glTexCoord2f((float)i/M, (float)j/N);
            glVertex3f(x0,y0,z0);

            // ring 1
            float x1 = (R + r*cv)*cu1;
            float y1 =  r*sv;
            float z1 = (R + r*cv)*su1;
            float nx1 = cv*cu1;
            float ny1 = sv;
            float nz1 = cv*su1;
            glNormal3f(nx1, ny1, nz1);
            glTexCoord2f((float)(i+1)/M, (float)j/N);
            glVertex3f(x1,y1,z1);
        }
        glEnd();
    }
}

void drawCone(int slices, float radius, float height)
{
    // side
    glBegin(GL_TRIANGLE_STRIP);
    for(int i=0;i<=slices;++i){
        float a = (float)i/slices * 2*PI;
        float cx = cos(a), sz = sin(a);
        // normal of cone side (approx): normalize(vec(radius*cx, height, radius*sz) x around)
        float nx = cx*height;
        float ny = radius;
        float nz = sz*height;
        float inv = 1.0f / std::sqrt(nx*nx+ny*ny+nz*nz);
        nx*=inv; ny*=inv; nz*=inv;

        glNormal3f(nx,ny,nz);
        glTexCoord2f((float)i/slices, 0.0f);
        glVertex3f(radius*cx, 0.0f, radius*sz);

        glNormal3f(nx,ny,nz);
        glTexCoord2f((float)i/slices, 1.0f);
        glVertex3f(0.0f, height, 0.0f);
    }
    glEnd();

    // base (circle)
    glNormal3f(0,-1,0);
    glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5f,0.5f); glVertex3f(0,0,0);
      for(int i=0;i<=slices;++i){
          float a = (float)i/slices * 2*PI;
          float x = cos(a), z = sin(a);
          glTexCoord2f(0.5f+0.5f*x, 0.5f+0.5f*z);
          glVertex3f(radius*x, 0.0f, radius*z);
      }
    glEnd();
}

void drawWavyGround(int N, float size, float amp, float texScale)
{
    // N grid quads per side centered at origin
    float half = size*0.5f;
    for (int i=0;i<N; ++i)
    {
        float x0 = -half + size*(float)i/N;
        float x1 = -half + size*(float)(i+1)/N;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j=0;j<=N; ++j)
        {
            float z  = -half + size*(float)j/N;

            float y0 = amp * std::sin(0.7f*x0) * std::cos(0.7f*z);
            float y1 = amp * std::sin(0.7f*x1) * std::cos(0.7f*z);

            // approximate normals via partial derivatives
            float nx0 = -0.7f*amp*std::cos(0.7f*x0)*std::cos(0.7f*z);
            float nz0 =  0.7f*amp*std::sin(0.7f*x0)*std::sin(0.7f*z);
            float len0= std::sqrt(nx0*nx0 + 1.0f + nz0*nz0);
            nx0/=len0; float ny0=1.0f/len0; nz0/=len0;

            float nx1 = -0.7f*amp*std::cos(0.7f*x1)*std::cos(0.7f*z);
            float nz1 =  0.7f*amp*std::sin(0.7f*x1)*std::sin(0.7f*z);
            float len1= std::sqrt(nx1*nx1 + 1.0f + nz1*nz1);
            nx1/=len1; float ny1=1.0f/len1; nz1/=len1;

            glNormal3f(nx0, ny0, nz0);
            glTexCoord2f(texScale*(x0/size+0.5f), texScale*(z/size+0.5f));
            glVertex3f(x0, y0, z);

            glNormal3f(nx1, ny1, nz1);
            glTexCoord2f(texScale*(x1/size+0.5f), texScale*(z/size+0.5f));
            glVertex3f(x1, y1, z);
        }
        glEnd();
    }
}
