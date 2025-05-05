COMPILER = clang
FLAGS = -Wall -Wextra -Wconversion -Wunreachable-code -Wpointer-arith -O3
LIBS = -lm -lcglm -Lgl -Llib -l:glad.a -lSDL2main -lSDL2 -Iinclude/stb

SRC = $(wildcard src/*/*.c) $(wildcard src/*.c) 

OUTPUT = .
BINNAME = main

DEFS = -D 'DEBUG_GL_SPECS=0' -D 'DEBUG_BATCH_VAO=0'

all: 
	$(COMPILER) $(SRC) -o $(OUTPUT)/$(BINNAME) $(LIBS) $(FLAGS) $(DEFS)
