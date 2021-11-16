SOURCE = source/main.cpp
LIBS = source/bin/libRSGL.so
GXX = g++
OUTPUT = Connect4

all:
	$(GXX) $(LIBS) $(SOURCE) -o $(OUTPUT)