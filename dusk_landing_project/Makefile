# Makefile for Dusk Landing - cross-platform OpenGL build

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = dusk_landing
SOURCES  = dusk_landing.cpp

PKG_CONFIG ?= pkg-config
GLEW_CFLAGS := $(shell $(PKG_CONFIG) --cflags glew 2>/dev/null)
GLEW_LIBS   := $(shell $(PKG_CONFIG) --libs glew 2>/dev/null)

CXXFLAGS += $(GLEW_CFLAGS)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lGL -lGLU -lglut
endif

ifneq ($(strip $(GLEW_LIBS)),)
	LIBS += $(GLEW_LIBS)
else ifneq ($(UNAME_S),Darwin)
	LIBS += -lGLEW
endif

all: $(TARGET)

$(TARGET): $(SOURCES)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)
	@echo "Build complete! Run with: ./$(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET)
	@echo "Clean complete."

distclean: clean
	rm -f *~ *.swp

help:
	@echo "Dusk Landing - Makefile Commands"
	@echo "================================"
	@echo "  make           - Build the project"
	@echo "  make run       - Build and run the program"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make distclean - Remove all generated files"
	@echo "  make help      - Show this help message"

.PHONY: all run clean distclean help
