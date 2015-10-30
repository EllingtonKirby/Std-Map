#include "Map.hpp"
#include <iostream>
using namespace cs540;

int main(int argc, char** argv){
	Map<int, char> default_constructed;	
	default_constructed.insert(std::pair<int, char>(5, 'A'));
	
	
	Map<char, int> second;
}
