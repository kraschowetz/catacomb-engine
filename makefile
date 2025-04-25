COMPILER = clang
FLAGS = -std=c99 -Wall -Wextra -Wconversion -Wunreachable-code -Wpointer-arith -O3
LIBS = -lm -lcglm -Lgl -Llib -l:glad.a -lSDL2main -lSDL2 -Iinclude/stb

SRC = $(wildcard src/*/*.c) $(wildcard src/*.c) 

OUTPUT = .
BINNAME = main

all: 
	$(COMPILER) $(SRC) -o $(OUTPUT)/$(BINNAME) $(LIBS) $(FLAGS)
