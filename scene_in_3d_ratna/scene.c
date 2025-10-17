/* ...existing code... */
#ifdef _WIN32
  #include <windows.h>
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
#elif __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef GLUT_KEY_PAGE_UP
#define GLUT_KEY_PAGE_UP 104
#endif
#ifndef GLUT_KEY_PAGE_DOWN
#define GLUT_KEY_PAGE_DOWN 105
#endif


static int   gW = 1200, gH = 800;
static float gYaw = 30.f, gPitch = 20.f;
static float gDist = 18.f;
static int   gPerspective = 0;
static float gOrthoHalf = 10.f;

static Mesh gTorus;
static Mesh gSuper;


static void draw_axes(float L){
  glLineWidth(1.0f);
  glBegin(GL_LINES);
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(L,0,0);
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,L,0);
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,L);
  glEnd();
}

static void set_projection(void){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float asp = (gH>0) ? (float)gW/(float)gH : 1.0f;
  if(gPerspective){
    gluPerspective(55.0, asp, 0.1, 1000.0);
  }else{
    float s = gOrthoHalf;
    if (asp >= 1.0f)
      glOrtho(-s*asp, s*asp, -s, s, -200.0, 200.0);
    else
      glOrtho(-s, s, -s/asp, s/asp, -200.0, 200.0);
  }
  glMatrixMode(GL_MODELVIEW);
}

static void draw_mesh_instanced(const Mesh* m,
                                float tx,float ty,float tz,
                                float rx,float ry,float rz,
                                float sx,float sy,float sz,
                                float cr,float cg,float cb)
{
  glPushMatrix();
    glTranslatef(tx,ty,tz);
    glRotatef(rz,0,0,1);
    glRotatef(ry,0,1,0);
    glRotatef(rx,1,0,0);
    glScalef(sx,sy,sz);
    glColor3f(cr,cg,cb);
    mesh_draw_triangles(m);
  glPopMatrix();
}


static void display(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  set_projection();

  glLoadIdentity();
  if(gPerspective){
    float cy = (float)(gDist*cos(gPitch*M_PI/180.0)*cos(gYaw*M_PI/180.0));
    float cx = (float)(gDist*cos(gPitch*M_PI/180.0)*sin(gYaw*M_PI/180.0));
    float cz = (float)(gDist*sin(gPitch*M_PI/180.0));
    gluLookAt(cx,cz,cy,  0,0,0,  0,1,0);
  }else{
    glRotatef(gPitch,1,0,0);
    glRotatef(gYaw,0,1,0);
  }

  draw_axes(2.0f);


  glColor3f(0.25f,0.25f,0.28f);
  glBegin(GL_QUADS);
    glVertex3f(-12, -2.2f, -12);
    glVertex3f( 12, -2.2f, -12);
    glVertex3f( 12, -2.2f,  12);
    glVertex3f(-12, -2.2f,  12);
  glEnd();

  draw_mesh_instanced(&gTorus, 0.0f, 0.0f, 0.0f,   0, 0, 0,   1,1,1,   0.95f,0.5f,0.2f);


  draw_mesh_instanced(&gSuper, -6.0f,-2.0f,-3.0f,  0, 20, 0,   2.0f,1.2f,2.0f,   0.3f,0.7f,0.9f);
  draw_mesh_instanced(&gSuper, -2.0f,-2.0f, 5.0f,  0,-30, 0,   1.2f,2.4f,1.2f,   0.9f,0.7f,0.3f);
  draw_mesh_instanced(&gSuper,  6.0f,-2.0f,-5.0f,  0, 60, 0,   1.6f,1.6f,2.6f,   0.5f,0.9f,0.5f);
  draw_mesh_instanced(&gSuper,  3.0f,-2.0f, 2.0f,  0,-10, 0,   2.4f,1.1f,1.1f,   0.8f,0.5f,0.9f);

  glutSwapBuffers();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, gW, 0, gH);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glColor3f(1,1,1);
  const char* hud = "Arrows: rotate | PgUp/PgDn: zoom | p: perspective | r: reset";
  glRasterPos2i(20, 30);
  for(const char* c=hud; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

static void reshape(int w,int h){
  gW = (w>1)? w:1;
  gH = (h>1)? h:1;
  glViewport(0,0,gW,gH);
  set_projection();
  glutPostRedisplay();
}

static void special(int key,int x,int y){
  (void)x; (void)y;
  switch(key){
    case GLUT_KEY_RIGHT: gYaw += 5.f; break;
    case GLUT_KEY_LEFT:  gYaw -= 5.f; break;
    case GLUT_KEY_UP:    gPitch += 5.f; if(gPitch>89.f) gPitch=89.f; break;
    case GLUT_KEY_DOWN:  gPitch -= 5.f; if(gPitch<-89.f) gPitch=-89.f; break;
    case GLUT_KEY_PAGE_UP:
      if(gPerspective){ gDist *= 0.9f; if(gDist<2.0f) gDist=2.0f; }
      else { gOrthoHalf *= 0.9f; if(gOrthoHalf<2.0f) gOrthoHalf=2.0f; }
      break;
    case GLUT_KEY_PAGE_DOWN:
      if(gPerspective){ gDist *= 1.1f; if(gDist>200.0f) gDist=200.0f; }
      else { gOrthoHalf *= 1.1f; if(gOrthoHalf>80.0f) gOrthoHalf=80.0f; }
      break;
  }
  glutPostRedisplay();
}

static void keyboard(unsigned char k,int x,int y){
  (void)x; (void)y;
  switch(k){
    case 27: mesh_free(&gTorus); mesh_free(&gSuper); exit(0);
    case 'p': case 'P': gPerspective ^= 1; glutPostRedisplay(); break;
    case 'r': case 'R': gYaw=30.f; gPitch=20.f; gDist=18.f; gOrthoHalf=10.f; glutPostRedisplay(); break;
  }
}


int main(int argc,char** argv){
  
  gTorus = mesh_make_twisted_torus(80,80, 5.0f,1.2f, 2);
  gSuper = mesh_make_superellipsoid(50,60, 1.0f,1.0f,1.0f, 0.4f,0.4f);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(gW,gH);
  glutCreateWindow("Procedural 3D Scene — Instancing & View Control");

  glClearColor(0.05f,0.06f,0.08f,1.0f);
  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
