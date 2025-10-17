# Makefile — build portable OpenGL/GLUT program (split files)
CC      = clang
CFLAGS  = -Wall -Wextra -O2 -std=c99 -DGL_SILENCE_DEPRECATION -Wno-deprecated-declarations
LIBS    = -framework OpenGL -framework GLUT -lm
# For Linux/Ubuntu (grader), use instead:
# LIBS    = -lglut -lGLU -lGL -lm

all: scene

scene: scene.o mesh.o
	$(CC) -o $@ $^ $(LIBS)

scene.o: scene.c mesh.h
	$(CC) $(CFLAGS) -c $< -o $@

mesh.o: mesh.c mesh.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f scene scene.o mesh.o
