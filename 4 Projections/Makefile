# Makefile

APP := hw4
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wno-deprecated-declarations -I./src -I./shaders -I/opt/homebrew/include -I/usr/local/include
LDFLAGS :=

SRC := src/main.cpp
OBJ := $(SRC:.cpp=.o)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
# macOS: link against system OpenGL framework and GLFW from Homebrew
LDFLAGS += -L/opt/homebrew/lib -L/usr/local/lib -Wl,-rpath,/opt/homebrew/lib -Wl,-rpath,/usr/local/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
# Linux
LDFLAGS += -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi
endif

.PHONY: all clean run

all: $(APP)

$(APP): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(APP)

clean:
	rm -f $(OBJ) $(APP)