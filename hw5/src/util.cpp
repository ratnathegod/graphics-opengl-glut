#include "util.hpp"

void hudPrintf(float x, float y, const char* fmt, ...)
{
    char buf[1024];
    va_list args; va_start(args,fmt);
    vsnprintf(buf,sizeof(buf),fmt,args); va_end(args);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(0,1,0,1,-1,1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glColor3f(1,1,1);
    glRasterPos3f(x,y,0);
    for (const char* p=buf; *p; ++p) glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*p);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

void drawNormalLine(float x,float y,float z,float nx,float ny,float nz,float s)
{ glBegin(GL_LINES); glVertex3f(x,y,z); glVertex3f(x+nx*s,y+ny*s,z+nz*s); glEnd(); }

void normalFromTriangle(const float A[3], const float B[3], const float C[3])
{
    float U[3]={B[0]-A[0],B[1]-A[1],B[2]-A[2]};
    float V[3]={C[0]-A[0],C[1]-A[1],C[2]-A[2]};
    float N[3]={ U[1]*V[2]-U[2]*V[1], U[2]*V[0]-U[0]*V[2], U[0]*V[1]-U[1]*V[0] };
    float len = std::sqrt(N[0]*N[0]+N[1]*N[1]+N[2]*N[2])+1e-9f;
    glNormal3f(N[0]/len, N[1]/len, N[2]/len);
}