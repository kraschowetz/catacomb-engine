CXX = clang
CXXFLAGS = -Wall -Wextra -Wconversion -Wunreachable-code
CXXFLAGS += -Wpointer-arith -std=c++20 -g
CXXFLAGS += -Iinclude
# TODO: add a real switch between release & debug mode
CXXFLAGS += -D DEVELOP -D LINUX
LDFLAGS = -Llib
#-l:glad.a -Iinclude/stb

SRC = $(wildcard src/*.cpp) $(wildcard src/cat/*/*.cpp)
OBJ = $(SRC:.cpp=.o)

OUTPUT = libcatacomb.a

.PHONY: build clean

build: dirs bin

dirs:
	mkdir -p ./bin

run: build
	cd sandbox; make run

rund: build
	cd sandbox; make run DEBUG=1

bin: $(OBJ)
	ar cr bin/$(OUTPUT) $(OBJ) #$^

%.o: %.cpp
	$(CXX) -c -o $@ -fpic $< $(CXXFLAGS)

clean:
	rm $(OBJ)

deep-clean:
	rm -rf bin $(OBJ)
