CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

SRC = src/SimpleImageLib.cpp src/main.cpp
OUT = image_processor

all:
    $(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
    rm -f $(OUT)
