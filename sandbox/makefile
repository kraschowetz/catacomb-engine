CXX = g++
CXXFLAGS = -Wall -Wextra -Wconversion -Wunreachable-code
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -Wpointer-arith -std=c++20 -O3
CXXFLAGS += `sdl2-config --cflags --libs`
LDFLAGS = -I../include -I../dependencies -L../lib -L../bin -lcatacomb -lSDL2 -l:glad.a
#LDFLAGS = -lSDL2main #only needed on windows
#LDFLAGS += `pkg-config --cflags assimp` `pkg-config --libs assimp`

DEBUG=0

DEBUG_FLAGS = -g -O0

DEBUG_FLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
LSAN_FLAGS = -fsanitize=address,undefined

SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*.c)
OUTPUT = main

.PHONY: all clear run

all: dir
ifeq ($(DEBUG), 0)
	$(CXX) $(SRC) -o bin/$(OUTPUT) $(CXXFLAGS) $(LDFLAGS)
else
	$(CXX) $(SRC) -o bin/$(OUTPUT) $(CXXFLAGS) $(DEBUG_FLAGS) $(LDFLAGS) $(LSAN_FLAGS)
endif

clear:
	rm -rf ./bin

run: all
ifeq ($(DEBUG), 0)
	./bin/$(OUTPUT)
else
	ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=1 LSAN_OPTIONS=suppressions=$(PWD)/lsan_suppressions.txt ./bin/$(OUTPUT)
endif

dir:
	mkdir -p ./bin
