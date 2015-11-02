#include "Map.hpp"
#include <iostream>
using namespace cs540;

int main(int argc, char** argv){
	Map<int, char> default_constructed;	
	std::pair<const int, char> toins = std::make_pair(5, 'A');
	default_constructed.insert(toins);
	std::cout << "Finished insert" << std::endl;	

	auto it = default_constructed.find(5);
	std::cout << "Found something" << it->second << std::endl;
	default_constructed.at(5);
	default_constructed[10]++;
	default_constructed[1]++;
	default_constructed[6]++;
	default_constructed.at(6);
	for(auto iter = default_constructed.rbegin(); iter != default_constructed.rend(); iter++){
		std::cout << "reverse looping" << std::endl;
	}

	std::cout << default_constructed[10] << std::endl;

	Map<int, char> newly_constructed;
	newly_constructed.insert(default_constructed.begin(), default_constructed.end());

	newly_constructed.erase(10);
	newly_constructed.erase(1);
	std::cout << newly_constructed.size() << std::endl;
	
	Map<int, char> copied;
        copied = newly_constructed;	

	newly_constructed.clear();
	std::cout << newly_constructed.size() << std::endl;

}
