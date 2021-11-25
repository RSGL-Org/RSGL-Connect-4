SOURCE = source/main.cpp
LIBS = source/bin/libRSGL.so
GXX = g++
OUTPUT = Connect4

all:
	$(GXX) $(SOURCE) $(LIBS) -o $(OUTPUT)
	@chmod +x ./3ds/3ds.sh
	@./3ds/3ds.sh

3DS:
	@chmod +x ./3ds/3ds.sh
	@./3ds/3ds.sh

linux:
	$(GXX) $(SOURCE) $(LIBS) -o $(OUTPUT)