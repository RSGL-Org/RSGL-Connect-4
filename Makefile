SOURCE = source/main.cpp
LIBS = source/bin/libRSGL.so -pthread
GXX = g++
OUTPUT = Connect4

all:
	$(GXX) $(SOURCE) $(LIBS) -o $(OUTPUT)