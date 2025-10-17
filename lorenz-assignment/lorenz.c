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

#ifndef GLUT_KEY_PAGE_UP
#define GLUT_KEY_PAGE_UP 104
#endif
#ifndef GLUT_KEY_PAGE_DOWN
#define GLUT_KEY_PAGE_DOWN 105
#endif

static double sigma=10.0, beta=8.0/3.0, rho=28.0, dt=0.001;

#define MAX_STEPS 200000
static int steps=120000;
static float* pts=NULL;

static double x0=1, y0i=1, z0=1;
static int winW=1200, winH=800;
static float th=20.f, ph=25.f, zoom=1.0f, bounds=60.f;
static int showHelp=1;

static void lorenz_step(double* x,double* y,double* z,double h){
  double dx = sigma * (*y - *x);
  double dy = (*x)*(rho - *z) - *y;
  double dz = (*x)*(*y) - beta * (*z);
  *x += h*dx; *y += h*dy; *z += h*dz;
}

static void recompute(void){
  if(!pts){ pts=(float*)malloc(sizeof(float)*3*MAX_STEPS); if(!pts){fprintf(stderr,"OOM\n"); exit(1);} }
  double x=x0,y=y0i,z=z0,m=0;
  for(int i=0;i<steps;i++){
    pts[3*i+0]=(float)x; pts[3*i+1]=(float)y; pts[3*i+2]=(float)z;
    if(fabs(x)>m) m=fabs(x); if(fabs(y)>m) m=fabs(y); if(fabs(z)>m) m=fabs(z);
    lorenz_step(&x,&y,&z,dt);
  }
  bounds=(float)(m*1.2+5.0);
  glutPostRedisplay();
}

static void setProjection(void){
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  double asp=(winH>0)?(double)winW/(double)winH:1.0;
  gluPerspective(55.0, asp, 0.1, 2000.0);
  glMatrixMode(GL_MODELVIEW);
}

static void drawString(int x,int y,const char* s){
  glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0,winW,0,winH);
  glMatrixMode(GL_MODELVIEW);  glPushMatrix(); glLoadIdentity();
  glRasterPos2i(x,y); for(const char* p=s;*p;++p) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*p);
  glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

static void display(void){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glEnable(GL_DEPTH_TEST);
  setProjection(); glLoadIdentity(); glTranslatef(0,0,-(2.4f*bounds)/zoom); glRotatef(th,1,0,0); glRotatef(ph,0,1,0);
  glLineWidth(1.5f); glColor3f(1,1,1); glBegin(GL_LINE_STRIP);
  for(int i=0;i<steps;i++) glVertex3fv(&pts[3*i]); glEnd();
  if(showHelp){ char buf[256];
    snprintf(buf,sizeof(buf),"sigma=%.3g beta=%.3g rho=%.3g dt=%.4g steps=%d zoom=%.2f",sigma,beta,rho,dt,steps,zoom);
    glColor3f(1,1,1); drawString(10,winH-20,buf);
    drawString(10,winH-38,"[Arrows] rotate  [PgUp/PgDn or +/-] zoom  [S/s][B/b][R/r] params  [,/.] dt  [1/2] steps  [h] help  [Esc] quit");
  }
  glutSwapBuffers();
}

static void reshape(int w,int h){ winW=w>1?w:1; winH=h>1?h:1; glViewport(0,0,winW,winH); setProjection(); glutPostRedisplay(); }

static void special(int key,int x,int y){
  if(key==GLUT_KEY_RIGHT) ph+=5; if(key==GLUT_KEY_LEFT) ph-=5;
  if(key==GLUT_KEY_UP) th+=5;    if(key==GLUT_KEY_DOWN) th-=5;
#if defined(GLUT_KEY_PAGE_UP)
  if(key==GLUT_KEY_PAGE_UP) zoom*=1.1f;
#endif
#if defined(GLUT_KEY_PAGE_DOWN)
  if(key==GLUT_KEY_PAGE_DOWN){ float z=zoom/1.1f; zoom=z<0.05f?0.05f:z; }
#endif
  glutPostRedisplay();
}

static void keyboard(unsigned char k,int x,int y){
  switch(k){
    case 27: free(pts); exit(0);
    case 'h': case 'H': showHelp=!showHelp; break;
    case 'S': sigma+=0.5; recompute(); break;   case 's': sigma-=0.5; recompute(); break;
    case 'B': beta +=0.1; recompute(); break;   case 'b': beta -=0.1; if(beta<0.01) beta=0.01; recompute(); break;
    case 'R': rho  +=1.0; recompute(); break;   case 'r': rho  -=1.0; if(rho<0.0) rho=0.0;   recompute(); break;
    case '.': dt*=1.2; if(dt>0.02) dt=0.02; recompute(); break;
    case ',': dt/=1.2; if(dt<1e-5) dt=1e-5; recompute(); break;
    case '1': steps=(int)(steps*0.75); if(steps<2000) steps=2000; recompute(); break;
    case '2': steps=(int)(steps*1.30); if(steps>MAX_STEPS) steps=MAX_STEPS; recompute(); break;
    case '+': case '=': zoom*=1.1f; glutPostRedisplay(); break;
    case '-': case '_': { float z=zoom/1.1f; zoom=z<0.05f?0.05f:z; glutPostRedisplay(); break; }
  }
}

int main(int argc,char** argv){
  if(argc>=2) steps=atoi(argv[1]);
  if(argc>=3) dt   =atof(argv[2]);
  if(argc>=4) sigma=atof(argv[3]);
  if(argc>=5) beta =atof(argv[4]);
  if(argc>=6) rho  =atof(argv[5]);

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(winW,winH);
  glutCreateWindow("Lorenz Attractor");
  glClearColor(0.02f,0.02f,0.03f,1.0f);

  recompute();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMainLoop();
  return 0;
}
