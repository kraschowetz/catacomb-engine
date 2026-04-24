CXX = clang++
CXXFLAGS = -Wall -Wextra -Wconversion -Wunreachable-code
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -Wpointer-arith -std=c++20
CXXFLAGS += `sdl2-config --cflags --libs`
LDFLAGS = -I../include -I../dependencies -L../lib -L../bin -lm -lcatacomb -lSDL2 -l:glad.a
#LDFLAGS = -lSDL2main #only needed on windows
#LDFLAGS += `pkg-config --cflags assimp` `pkg-config --libs assimp`

DEBUG=1

DBGLV_FLAGS_0 = -O3

DBGLV_FLAGS_1 = -g -O1 -D DEVELOP -D ENABLE_BENCHMARK

DBGLV_FLAGS_2 = -g -O0 -D DEVELOP -D ENABLE_BENCHMARK

LSAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*.c)
OUTPUT = main

.PHONY: all clear run

all: dir
ifeq ($(DEBUG), 0)
	$(CXX) $(SRC) -o bin/$(OUTPUT) $(CXXFLAGS) $(DBGLV_FLAGS_0) $(LDFLAGS)
else ifeq ($(DEBUG), 1)
	$(CXX) $(SRC) -o bin/$(OUTPUT) $(CXXFLAGS) $(DBGLV_FLAGS_1) $(LDFLAGS)
else ifeq ($(DEBUG), 2)
	$(CXX) $(SRC) -o bin/$(OUTPUT) $(CXXFLAGS) $(DBGLV_FLAGS_2) $(LDFLAGS) $(LSAN_FLAGS)
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
