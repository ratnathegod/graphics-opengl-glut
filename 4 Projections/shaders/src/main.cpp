#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLuint compileShader(GLenum type, const char* src){
GLuint sh = glCreateShader(type);
glShaderSource(sh,1,&src,nullptr);
glCompileShader(sh);
GLint ok=0; glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);
if(!ok){ char log[1024]; glGetShaderInfoLog(sh,1024,nullptr,log); fprintf(stderr,"Shader error: %s\n",log); }
return sh;
}

static GLuint linkProgram(const char* vsSrc, const char* fsSrc){
GLuint vs = compileShader(GL_VERTEX_SHADER, vsSrc);
GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
GLuint prog = glCreateProgram();
glAttachShader(prog, vs);
glAttachShader(prog, fs);
glLinkProgram(prog);
GLint ok=0; glGetProgramiv(prog,GL_LINK_STATUS,&ok);
if(!ok){ char log[1024]; glGetProgramInfoLog(prog,1024,nullptr,log); fprintf(stderr,"Link error: %s\n",log); }
glDeleteShader(vs); glDeleteShader(fs);
return prog;
}

static const char* VERT_SRC = R"GLSL(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
uniform mat4 uMVP;
out vec3 vColor;
void main(){ gl_Position = uMVP * vec4(aPos,1.0); vColor = aColor; }
)GLSL";

static const char* FRAG_SRC = R"GLSL(
#version 330 core
in vec3 vColor; out vec4 FragColor; void main(){ FragColor = vec4(vColor,1.0); }
)GLSL";

struct Mesh { GLuint vao=0,vbo=0; GLsizei count=0; };

static Mesh makeGrid(int N=20, float s=1.0f){
std::vector<float> v; v.reserve((N+1)*4*6);
float half = N*s*0.5f;
for(int i=0;i<=N;i++){
float t = -half + i*s;
v.insert(v.end(), { -half,0.0f,t, 0.6f,0.6f,0.6f, half,0.0f,t, 0.6f,0.6f,0.6f });
v.insert(v.end(), { t,0.0f,-half, 0.6f,0.6f,0.6f, t,0.0f, half, 0.6f,0.6f,0.6f });
}
Mesh m; glGenVertexArrays(1,&m.vao); glGenBuffers(1,&m.vbo);
glBindVertexArray(m.vao);
glBindBuffer(GL_ARRAY_BUFFER,m.vbo);
glBufferData(GL_ARRAY_BUFFER,v.size()*sizeof(float),v.data(),GL_STATIC_DRAW);
glEnableVertexAttribArray(0); glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
glEnableVertexAttribArray(1); glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
m.count = (GLsizei)(v.size()/6);
glBindVertexArray(0);
return m;
}