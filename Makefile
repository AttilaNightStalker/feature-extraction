SOURCE := main.cpp
OBJ := main.o

CFLAGS = -Wall -O3 -fPIC -I. `pkg-config --cflags opencv`

LIB = `pkg-config --libs opencv`

all:
	g++ $(CFLAGS) $(LIB) ./extraction.so -o main $(SOURCE)

extraction:
	g++ -shared $(CFLAGS) $(LIB) -o extraction.so extraction.cpp

