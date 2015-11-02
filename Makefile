COMP= g++
FLAGS = -g -Wall -std=c++0x
PROJ = testMap

test: test.o
	$(COMP) $(FLAGS) -o $(PROJ) test.o

test.o: test.cpp
	$(COMP) $(FLAGS) -c test.cpp

self_test: self_test.o
	$(COMP) $(FLAGS) -o $(PROJ) self_test.o

self_test.o: self_test.cpp
	$(COMP) $(FLAGS) -c self_test.cpp

Map: Map.hpp
	$(COMP) $(FLAGS) -c Map.hpp

clean:
	rm *.o $(PROJ)
