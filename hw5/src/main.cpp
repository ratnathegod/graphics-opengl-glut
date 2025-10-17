// Modular HW5: OpenGL Lighting with Custom Normals (no GLU/GLUT objects)
#include "common.hpp"
#include "camera.hpp"
#include "lighting.hpp"
#include "geometry.hpp"
#include "util.hpp"

static CameraState cam; 
static LightState  L;
static int lastTime=0; // ms

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glEnable(GL_DEPTH_TEST);
    applyProjection(cam); applyView(cam);
    setupLighting(L);

    // Scene
    drawGrid(12,0.5);

    glPushMatrix(); glTranslatef(-2.5f,1.0f,0.0f); glRotatef(90,1,0,0); glColor3f(0.8f,0.2f,0.2f); drawTorus(1.2,0.4,64,32); glPopMatrix();
    glPushMatrix(); glTranslatef(2.2f,0.2f,-0.5f); glRotatef(-20,0,1,0); glColor3f(0.2f,0.6f,0.9f); drawHelicoid(1.5,0.25,40,80); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f,0.8f,2.5f); glRotatef(35,0,1,0); glColor3f(0.6f,0.5f,0.4f); drawRock(1.2); glPopMatrix();

    hudPrintf(0.02f,0.96f,"th=%d ph=%d  mode=%d  light(%s)  theta=%.0f elev=%.0f",cam.th,cam.ph,cam.mode,L.enabled?"on":"off",L.theta,L.elev);

    glutSwapBuffers();
}

static void idle()
{
    int t = glutGet(GLUT_ELAPSED_TIME); float dt = (t-lastTime)*0.001f; lastTime=t; 
    tickLight(L,dt); glutPostRedisplay();
}

static void key(unsigned char ch,int,int)
{
    switch(ch){
        case 27: case 'q': case 'Q': std::exit(0); break;
        case '0': cam.th=cam.ph=0; break;
        case 'm': cam.mode=(cam.mode+1)%3; break;
        case 'n': gShowNormals = 1-gShowNormals; break;
        case 'l': L.enabled = 1-L.enabled; break;
        case ' ': L.animate = 1-L.animate; break;
        case '[': L.theta -= 5; break; case ']': L.theta += 5; break;
        case ';': L.elev  -= 5; break; case '\\': L.elev  += 5; break;
        case '-': cam.fov = (cam.fov>10)? cam.fov-2 : cam.fov; break;
        case '+': case '=': cam.fov = (cam.fov<120)? cam.fov+2 : cam.fov; break;
        case ',': L.R = (L.R>2.0)? L.R-0.5 : L.R; break;
        case '.': L.R = (L.R<20.0)? L.R+0.5 : L.R; break;
    }
    applyProjection(cam); glutPostRedisplay();
}

static void special(int k,int,int)
{
    if (k==GLUT_KEY_RIGHT) cam.th += 5; else if (k==GLUT_KEY_LEFT) cam.th -= 5;
    else if (k==GLUT_KEY_UP) cam.ph += 5; else if (k==GLUT_KEY_DOWN) cam.ph -= 5;
    cam.th%=360; cam.ph%=360; applyProjection(cam); glutPostRedisplay();
}

static void reshape(int w,int h){ cam.asp = (h>0)? (double)w/h : 1.0; glViewport(0,0,w,h); applyProjection(cam); }

static void init()
{ glClearColor(0.05f,0.07f,0.1f,1); glEnable(GL_CULL_FACE); }

int main(int argc,char** argv)
{
    glutInit(&argc,argv); glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(1000,700); glutCreateWindow("HW5 (Modular): Lighting + Custom Normals");
    init(); glutDisplayFunc(display); glutReshapeFunc(reshape); glutKeyboardFunc(key); glutSpecialFunc(special); glutIdleFunc(idle); glutMainLoop(); return 0;
}