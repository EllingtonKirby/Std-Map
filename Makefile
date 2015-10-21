COMP= g++
FLAGS = -g -Wall -std=c++0x
PROJ = map

all: test-kec.o
	$(COMP) $(FLAGS) -o $(PROJ) test-kec.o

test-kec.o: test-kec.cpp
	$(COMP) $(FLAGS) -c test-kec.cpp

Map: Map.hpp
	$(COMP) $(FLAGS) -c Map.hpp

clean:
	rm *.o *.gch $(PROJ)
