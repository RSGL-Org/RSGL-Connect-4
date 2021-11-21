SOURCE = source/main.cpp
LIBS = source/bin/libRSGL.so source/bin/libpthread-2.33.so
GXX = g++
OUTPUT = Connect4

all:
	$(GXX) $(SOURCE) $(LIBS) -o $(OUTPUT)
