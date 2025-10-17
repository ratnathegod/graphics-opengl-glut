// main.cpp
// Textured + lit scene with user control (no GLU/GLUT objects)
// Build: see Makefile

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "math.hpp"
#include "mesh.hpp" 
#include "texture.hpp"

// Constants
constexpr float FOV = 55.0f * DEG2RAD;
constexpr float CLEAR_R = 0.06f, CLEAR_G = 0.07f, CLEAR_B = 0.09f;
constexpr int TORUS_SEGMENTS = 64, TORUS_RINGS = 24;
constexpr int CONE_SEGMENTS = 72;
constexpr int GROUND_SIZE = 120;
constexpr float LIGHT_ANIM_SPEED = 0.5f * DEG2RAD;
constexpr float CAM_ROTATE_STEP = 5.0f * DEG2RAD;
constexpr float CAM_PITCH_STEP = 3.0f * DEG2RAD;

// Simple POD structs (no new files)
struct OrbitCam { 
    float theta, phi, radius; 
};

struct Light { 
    float angle, radius, height; 
};

struct AppState {
    OrbitCam cam;
    Light light;
    bool useLighting, useTextures, animate;
    int winW, winH;
};

// Global state and textures
static AppState state = {
    {45.0f * DEG2RAD, 20.0f * DEG2RAD, 5.0f}, // cam: theta, phi, radius
    {0.0f, 8.0f, 4.0f},                       // light: angle, radius, height
    true, true, true,                          // useLighting, useTextures, animate
    1024, 720                                  // winW, winH
};
static GLuint texChecker = 0;
static GLuint texStripes = 0;

// Set up projection matrix
static void setProjection(const AppState& st)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    Mat4 P = perspective(FOV, (float)st.winW / (float)st.winH, 0.1f, 100.0f);
    glMultMatrixf(P.m);
    glMatrixMode(GL_MODELVIEW);
}

// Apply camera transform using lookAt
static void applyCamera(const AppState& st)
{
    glLoadIdentity();
    float cx = st.cam.radius * std::cos(st.cam.phi) * std::cos(st.cam.theta);
    float cy = st.cam.radius * std::sin(st.cam.phi);
    float cz = st.cam.radius * std::cos(st.cam.phi) * std::sin(st.cam.theta);
    
    Mat4 V = lookAt(Vec3{cx, cy, cz}, Vec3{0,0,0}, Vec3{0,1,0});
    glMultMatrixf(V.m);
}

// Configure lighting based on current light state
static void applyLighting(const AppState& st)
{
    if (!st.useLighting) { 
        glDisable(GL_LIGHTING); 
        return; 
    }
    
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    // White positional light orbiting around Y
    float lx = st.light.radius * std::cos(st.light.angle);
    float lz = st.light.radius * std::sin(st.light.angle);
    float pos[4] = { lx, st.light.height, lz, 1.0f };
    float amb[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
    float dif[4] = { 0.95f, 0.95f, 0.95f, 1.0f };
    float spe[4] = { 1.00f, 1.00f, 1.00f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT , amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE , dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
    glEnable(GL_LIGHT0);

    // Additional white directional light
    float dpos[4] = { 0.3f, 1.0f, 0.4f, 0.0f };
    float ddif[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float damb[4] = { 0.05f, 0.05f, 0.05f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, dpos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE , ddif);
    glLightfv(GL_LIGHT1, GL_AMBIENT , damb);
    glEnable(GL_LIGHT1);
}

// Draw coordinate axes for reference
static void drawAxes(float length = 2.0f)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
      glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(length,0,0); // X
      glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,length,0); // Y
      glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,length); // Z
    glEnd();
    glColor3f(1,1,1);
}

// Draw the main scene with all objects
static void drawScene(const AppState& st, GLuint texChecker, GLuint texStripes)
{
    if (st.useTextures) glEnable(GL_TEXTURE_2D); 
    else glDisable(GL_TEXTURE_2D);
    
    // Ground (textured wavy grid)
    if (st.useTextures) glBindTexture(GL_TEXTURE_2D, texChecker);
    else glBindTexture(GL_TEXTURE_2D, 0);

    glPushMatrix();
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat[4]){0.9f,0.9f,0.9f,1});
      drawWavyGround(GROUND_SIZE, 12.0f, 0.25f, 2.0f);
    glPopMatrix();

    // Torus
    if (st.useTextures) glBindTexture(GL_TEXTURE_2D, texStripes);
    glPushMatrix();
      glTranslatef(-2.5f, 1.2f, 0.0f);
      glRotatef(-35.0f, 0,1,0);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat[4]){0.7f,0.75f,1.0f,1});
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat[4]){0.6f,0.6f,0.6f,1});
      glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 64.0f);
      drawTorus(TORUS_SEGMENTS, TORUS_RINGS, 1.2f, 0.35f);
    glPopMatrix();

    // Cone (stone-like)
    if (st.useTextures) glBindTexture(GL_TEXTURE_2D, texChecker);
    glPushMatrix();
      glTranslatef(2.4f, 0.0f, -0.5f);
      glRotatef(90.0f, -1,0,0);
      glTranslatef(0,0,0.75f);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat[4]){0.85f,0.8f,0.7f,1});
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat[4]){0.3f,0.3f,0.3f,1});
      glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 16.0f);
      drawCone(CONE_SEGMENTS, 0.8f, 1.6f);
    glPopMatrix();

    // Visible light position marker
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.9f, 0.6f);
    float lx = st.light.radius * std::cos(st.light.angle);
    float lz = st.light.radius * std::sin(st.light.angle);
    glPushMatrix();
      glTranslatef(lx, st.light.height, lz);
      glutSolidCube(0.12f);
    glPopMatrix();
    if (st.useLighting) glEnable(GL_LIGHTING);
    glColor3f(1,1,1);
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    setProjection(state);
    applyCamera(state);
    applyLighting(state);

    drawAxes(1.5f);
    drawScene(state, texChecker, texStripes);

    glutSwapBuffers();
}

static void reshape(int w, int h)
{
    state.winW = (w>1)?w:1; 
    state.winH = (h>1)?h:1;
    glViewport(0, 0, state.winW, state.winH);
    setProjection(state);
    glutPostRedisplay();
}

static void idle()
{
    if (state.animate) {
        state.light.angle += LIGHT_ANIM_SPEED;
        if (state.light.angle > 2.0f*PI) state.light.angle -= 2.0f*PI;
        glutPostRedisplay();
    }
}

static void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
    case 27: case 'q': std::exit(0); break;
    case 'l': state.useLighting = !state.useLighting; glutPostRedisplay(); break;
    case 't': state.useTextures = !state.useTextures; glutPostRedisplay(); break;
    case 'a': state.cam.theta -= CAM_ROTATE_STEP; glutPostRedisplay(); break;
    case 'd': state.cam.theta += CAM_ROTATE_STEP; glutPostRedisplay(); break;
    case 'w': state.cam.phi = clamp(state.cam.phi + CAM_PITCH_STEP, -80.0f*DEG2RAD, 80.0f*DEG2RAD); glutPostRedisplay(); break;
    case 's': state.cam.phi = clamp(state.cam.phi - CAM_PITCH_STEP, -80.0f*DEG2RAD, 80.0f*DEG2RAD); glutPostRedisplay(); break;
    case '+': state.light.radius += 0.2f; glutPostRedisplay(); break;
    case '-': state.light.radius = std::max(0.5f, state.light.radius-0.2f); glutPostRedisplay(); break;
    case ']': state.light.height += 0.2f; glutPostRedisplay(); break;
    case '[': state.light.height -= 0.2f; glutPostRedisplay(); break;
    case ' ': state.animate = !state.animate; break;
    }
}

static void mouse(int btn, int mouseState, int, int)
{
    if (btn==3 && mouseState==GLUT_DOWN) { 
        state.cam.radius = std::max(2.5f, state.cam.radius-0.5f); 
        glutPostRedisplay(); 
    }
    if (btn==4 && mouseState==GLUT_DOWN) { 
        state.cam.radius = std::min(30.0f, state.cam.radius+0.5f); 
        glutPostRedisplay(); 
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(state.winW, state.winH);
    glutCreateWindow("Lighting + Textures (GLU-free, no GLUT objects)");

    glEnable(GL_DEPTH_TEST);
    glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, 1.0f);
    glShadeModel(GL_SMOOTH);

    setProjection(state);

    // Materials default
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat[4]){0.25f,0.25f,0.25f,1});
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);

    // Textures (power-of-two)
    texChecker = makeCheckerTexture(256, 32);
    texStripes = makeStripeTexture (256, 16);

    // Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
