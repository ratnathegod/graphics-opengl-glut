#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Mesh { GLuint vao=0,vbo=0; GLsizei count=0; GLenum prim=GL_TRIANGLES; };

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

static Mesh makeGrid(int N=20, float s=1.0f){
	std::vector<float> v; v.reserve((N+1)*4*6);
	float half = N*s*0.5f;
	for(int i=0;i<=N;i++){
		float t = -half + i*s;
		v.insert(v.end(), { -half,0.0f,t, 0.6f,0.6f,0.6f,  half,0.0f,t, 0.6f,0.6f,0.6f });
		v.insert(v.end(), { t,0.0f,-half, 0.6f,0.6f,0.6f,  t,0.0f, half, 0.6f,0.6f,0.6f });
	}
	Mesh m; glGenVertexArrays(1,&m.vao); glGenBuffers(1,&m.vbo);
	glBindVertexArray(m.vao);
	glBindBuffer(GL_ARRAY_BUFFER,m.vbo);
	glBufferData(GL_ARRAY_BUFFER,v.size()*sizeof(float),v.data(),GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(1); glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
	m.count = (GLsizei)(v.size()/6);
	m.prim = GL_LINES;
	glBindVertexArray(0);
	return m;
}

static Mesh makeCube(glm::vec3 color){
	const float c[3] = {color.r,color.g,color.b};
	const float p = 0.5f;
	const float verts[] = {
		// +X
		p,-p,-p, c[0],c[1],c[2],  p, p,-p, c[0],c[1],c[2],  p, p, p, c[0],c[1],c[2],
		p,-p,-p, c[0],c[1],c[2],  p, p, p, c[0],c[1],c[2],  p,-p, p, c[0],c[1],c[2],
		// -X
		-p,-p,-p, c[0],c[1],c[2], -p,-p, p, c[0],c[1],c[2], -p, p, p, c[0],c[1],c[2],
		-p,-p,-p, c[0],c[1],c[2], -p, p, p, c[0],c[1],c[2], -p, p,-p, c[0],c[1],c[2],
		// +Y
		-p, p,-p, c[0],c[1],c[2], -p, p, p, c[0],c[1],c[2],  p, p, p, c[0],c[1],c[2],
		-p, p,-p, c[0],c[1],c[2],  p, p, p, c[0],c[1],c[2],  p, p,-p, c[0],c[1],c[2],
		// -Y
		-p,-p,-p, c[0],c[1],c[2],  p,-p,-p, c[0],c[1],c[2],  p,-p, p, c[0],c[1],c[2],
		-p,-p,-p, c[0],c[1],c[2],  p,-p, p, c[0],c[1],c[2], -p,-p, p, c[0],c[1],c[2],
		// +Z
		-p,-p, p, c[0],c[1],c[2],  p,-p, p, c[0],c[1],c[2],  p, p, p, c[0],c[1],c[2],
		-p,-p, p, c[0],c[1],c[2],  p, p, p, c[0],c[1],c[2], -p, p, p, c[0],c[1],c[2],
		// -Z
		-p,-p,-p, c[0],c[1],c[2], -p, p,-p, c[0],c[1],c[2],  p, p,-p, c[0],c[1],c[2],
		-p,-p,-p, c[0],c[1],c[2],  p, p,-p, c[0],c[1],c[2],  p,-p,-p, c[0],c[1],c[2],
	};
	Mesh m; glGenVertexArrays(1,&m.vao); glGenBuffers(1,&m.vbo);
	glBindVertexArray(m.vao);
	glBindBuffer(GL_ARRAY_BUFFER,m.vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(verts),verts,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(1); glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
	m.count = (GLsizei)(sizeof(verts)/ (6*sizeof(float)));
	m.prim = GL_TRIANGLES;
	glBindVertexArray(0);
	return m;
}

static void drawMesh(const Mesh& m, const glm::mat4& M, const glm::mat4& VP, GLuint prog){
	glm::mat4 MVP = VP * M;
	glUseProgram(prog);
	glUniformMatrix4fv(glGetUniformLocation(prog,"uMVP"),1,GL_FALSE,glm::value_ptr(MVP));
	glBindVertexArray(m.vao);
	glDrawArrays(m.prim,0,m.count);
	glBindVertexArray(0);
}

enum class Mode { Ortho=0, Persp=1, FPS=2 };

struct Orbit { float th=45.0f, ph=30.0f, radius=10.0f; glm::vec3 target{0,0,0}; };
struct FPSCam { glm::vec3 pos{0,1.2f,5.0f}; float yaw= -90.0f; float pitch=0.0f; };

static bool keys[1024] = {false};

static void keycb(GLFWwindow* w, int key, int sc, int action, int mods){
	if(key<0 || key>=1024) return;
	if(action==GLFW_PRESS) keys[key]=true;
	else if(action==GLFW_RELEASE) keys[key]=false;
}

int main(){
	if(!glfwInit()){ fprintf(stderr,"Failed to init GLFW\n"); return 1; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(1024,768,"HW4 - Ratnakaru Yalagathala - OpenGL Projections",nullptr,nullptr);
	if(!window){ fprintf(stderr,"Failed to create window\n"); glfwTerminate(); return 1; }
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window,keycb);

	GLuint prog = linkProgram(VERT_SRC, FRAG_SRC);

	Mesh grid = makeGrid(40, 0.5f);
	Mesh cubeR = makeCube({1,0,0});
	Mesh cubeG = makeCube({0,1,0});
	Mesh cubeB = makeCube({0,0,1});

	Mode mode = Mode::Ortho;
	Orbit orbit; FPSCam fps;

	double last = glfwGetTime();
	bool mPrev=false;
	while(!glfwWindowShouldClose(window)){
		double now = glfwGetTime();
		float dt = (float)(now - last); last = now;
		int w,h; glfwGetFramebufferSize(window,&w,&h); float aspect = (h>0)? (float)w/h : 1.0f;

		glfwPollEvents();
		if(keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window,1);
		if(keys[GLFW_KEY_R]){ orbit = Orbit{}; fps = FPSCam{}; }
		if(keys[GLFW_KEY_M] && !mPrev){ mode = (Mode)(((int)mode+1)%3); }
		mPrev = keys[GLFW_KEY_M];

		const float orbitStep = 90.0f*dt;
		const float zoomStep = 10.0f*dt;
		if(mode==Mode::Ortho || mode==Mode::Persp){
			if(keys[GLFW_KEY_LEFT]) orbit.th -= orbitStep;
			if(keys[GLFW_KEY_RIGHT]) orbit.th += orbitStep;
			if(keys[GLFW_KEY_UP]) orbit.ph = std::min(89.0f, orbit.ph + orbitStep);
			if(keys[GLFW_KEY_DOWN]) orbit.ph = std::max(-89.0f, orbit.ph - orbitStep);
			if(keys[GLFW_KEY_LEFT_BRACKET]) orbit.radius = std::max(3.0f, orbit.radius - zoomStep);
			if(keys[GLFW_KEY_RIGHT_BRACKET]) orbit.radius = std::min(50.0f, orbit.radius + zoomStep);
		} else {
			float speed = (keys[GLFW_KEY_LEFT_SHIFT]||keys[GLFW_KEY_RIGHT_SHIFT])? 6.0f : 3.0f;
			float yawRad = glm::radians(fps.yaw);
			float pitchRad = glm::radians(fps.pitch);
			glm::vec3 forward = glm::normalize(glm::vec3(cosf(yawRad)*cosf(pitchRad), sinf(pitchRad), sinf(yawRad)*cosf(pitchRad)));
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
			if(keys[GLFW_KEY_W]) fps.pos += forward * speed * dt;
			if(keys[GLFW_KEY_S]) fps.pos -= forward * speed * dt;
			if(keys[GLFW_KEY_A]) fps.pos -= right * speed * dt;
			if(keys[GLFW_KEY_D]) fps.pos += right * speed * dt;
			if(keys[GLFW_KEY_LEFT]) fps.yaw -= 90.0f*dt;
			if(keys[GLFW_KEY_RIGHT]) fps.yaw += 90.0f*dt;
			if(keys[GLFW_KEY_UP]) fps.pitch = std::min(89.0f, fps.pitch + 60.0f*dt);
			if(keys[GLFW_KEY_DOWN]) fps.pitch = std::max(-89.0f, fps.pitch - 60.0f*dt);
		}

		glm::mat4 V(1.0f), P(1.0f);
		if(mode==Mode::Ortho){
			float viewHeight = orbit.radius * 1.2f;
			float viewWidth = viewHeight * aspect;
			P = glm::ortho(-viewWidth, viewWidth, -viewHeight, viewHeight, -50.0f, 50.0f);
			float thR = glm::radians(orbit.th), phR = glm::radians(orbit.ph);
			glm::vec3 eye = orbit.target + glm::vec3(
				orbit.radius * cosf(phR) * cosf(thR),
				orbit.radius * sinf(phR),
				orbit.radius * cosf(phR) * sinf(thR)
			);
			V = glm::lookAt(eye, orbit.target, glm::vec3(0,1,0));
		} else if(mode==Mode::Persp){
			P = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
			float thR = glm::radians(orbit.th), phR = glm::radians(orbit.ph);
			glm::vec3 eye = orbit.target + glm::vec3(
				orbit.radius * cosf(phR) * cosf(thR),
				orbit.radius * sinf(phR),
				orbit.radius * cosf(phR) * sinf(thR)
			);
			V = glm::lookAt(eye, orbit.target, glm::vec3(0,1,0));
		} else {
			P = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
			float yawR = glm::radians(fps.yaw), pitchR = glm::radians(fps.pitch);
			glm::vec3 dir( cosf(yawR)*cosf(pitchR), sinf(pitchR), sinf(yawR)*cosf(pitchR) );
			V = glm::lookAt(fps.pos, fps.pos + glm::normalize(dir), glm::vec3(0,1,0));
		}
		glm::mat4 VP = P * V;

		glViewport(0,0,w,h);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.05f,0.08f,0.12f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		drawMesh(grid, glm::mat4(1.0f), VP, prog);
		drawMesh(cubeR, glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f,0.75f,-2.0f)), VP, prog);
		drawMesh(cubeG, glm::translate(glm::mat4(1.0f), glm::vec3( 2.5f,0.50f, 3.0f)), VP, prog);
		drawMesh(cubeB, glm::translate(glm::mat4(1.0f), glm::vec3( 0.0f,0.40f,-4.0f)), VP, prog);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}