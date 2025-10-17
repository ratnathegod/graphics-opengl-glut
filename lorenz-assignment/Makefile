CC      = clang
CFLAGS  = -Wall -Wextra -O2 -std=c99 -DGL_SILENCE_DEPRECATION -Wno-deprecated-declarations
LIBS    = -framework OpenGL -framework GLUT -lm

all: lorenz

lorenz: lorenz.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

lorenz.o: lorenz.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f lorenz lorenz.o
