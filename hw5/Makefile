# Modular HW5 Makefile (macOS + Linux)
APP=hw5
SRC=src/main.cpp src/util.cpp src/camera.cpp src/lighting.cpp src/geometry.cpp
HDR=src/common.hpp src/util.hpp src/camera.hpp src/lighting.hpp src/geometry.hpp
OBJ=$(SRC:.cpp=.o)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
CXX=clang++
CXXFLAGS=-O2 -std=c++17 -Wall -Wextra
LDFLAGS=-framework OpenGL -framework GLUT
$(APP): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LDFLAGS)
else
CXX=g++
CXXFLAGS=-O2 -std=c++17 -Wall -Wextra
LDLIBS=-lGL -lglut -lm
$(APP): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LDLIBS)
endif

.PHONY: clean run
clean:
	rm -f $(APP) $(OBJ)
run: $(APP)
	./$(APP)