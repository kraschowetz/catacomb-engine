COMPILER = clang++
FLAGS = -Wall -Wextra -Wconversion -Wunreachable-code -Wpointer-arith -O3
FLAGS += -lm -Wno-unused-parameter -std=c++20
LIBS = -Lgl -Llib -l:glad.a -lSDL2main -lSDL2 -Iinclude/stb

SRC = $(wildcard src/*/*.cpp) $(wildcard src/*.cpp) 

OUTPUT = .
BINNAME = main

DEFS = -D 'NO_ASSERTS=0'
DEFS += -D 'DEBUG_GL_SPECS=0' -D 'DEBUG_BATCH_VAO=0'

all: 
	$(COMPILER) $(SRC) -o $(OUTPUT)/$(BINNAME) $(LIBS) $(FLAGS) $(DEFS)
