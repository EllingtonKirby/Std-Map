#include "Map.hpp"
#include <iostream>
using namespace cs540;

int main(int argc, char** argv){
	Map<int, char> default_constructed;	
	std::pair<const int, char> toins = std::make_pair(5, 'A');
	default_constructed.insert(toins);
	default_constructed.erase(5);
	std::cout << "Finished insert" << std::endl;	
	Map<char, int> second;
	Map<int, char> copied;
	copied = default_constructed;
}
