CXX = clang++
CXXFLAGS = -Wall -Wextra -Wconversion -Wunreachable-code
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -Wpointer-arith -std=c++20
CXXFLAGS += -Iinclude
CXXFLAGS += -D DEVELOP -D LINUX -D ENABLE_BENCHMARK
LDFLAGS = -Llib

DEBUG=1

DBGLV_FLAGS_0 = -O3
DBGLV_FLAGS_1 = -g -O1 -D DEVELOP -D ENABLE_BENCHMARK
DBGLV_FLAGS_2 = -g -O0 -D DEVELOP -D ENABLE_BENCHMARK

SRC = $(wildcard src/*.cpp) $(wildcard src/cat/*/*.cpp)
OBJ = $(SRC:.cpp=.o)

OUTPUT = libcatacomb.a

.PHONY: build clean

build: dirs bin

dirs:
	mkdir -p ./bin

run: build
	cd sandbox; make run DEBUG=$(DEBUG)

rund: build
	cd sandbox; make run DEBUG=2

bin: $(OBJ)
	ar cr bin/$(OUTPUT) $(OBJ) #$^

%.o: %.cpp
ifeq ($(DEBUG), 0)
	$(CXX) -c -o $@ -fpic $< $(CXXFLAGS) $(DBGLV_FLAGS_0)
else ifeq ($(DEBUG), 1)
	$(CXX) -c -o $@ -fpic $< $(CXXFLAGS) $(DBGLV_FLAGS_1)
else ifeq ($(DEBUG), 2)
	$(CXX) -c -o $@ -fpic $< $(CXXFLAGS) $(DBGLV_FLAGS_2)
endif

clean:
	rm $(OBJ)

deep-clean:
	rm -rf bin $(OBJ)
