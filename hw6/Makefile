# Makefile – Linux/macOS (uses freeglut, GLU-free)

APP = textured_lighting
SRC = main.cpp mesh.cpp texture.cpp
HDR = math.hpp mesh.hpp texture.hpp
CXX = g++
CXXFLAGS = -O2 -std=c++17

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS with Homebrew's freeglut
    LIBS = -framework OpenGL -framework GLUT
else
    # Linux (GLU-free since we use glGenerateMipmap)
    LIBS = -lGL -lglut
endif

all: $(APP)

$(APP): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LIBS)

clean:
	rm -f $(APP)

zip: clean
	zip hw6.zip * -x __MACOSX\* -x .DS_Store
