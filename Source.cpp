#include "test01split.hpp"

#include <iostream>
#include <string>
#include <fstream>

// example of use
int main(int argc, char * * argv)
{
	using namespace splitter;

	std::istringstream iss("alpha:=10/50.1");

	std::string x;
	int y;
	double z;

	iss >> split(x, ':', '=', y, '/', z);

	std::cout << "x = " << x << ", y = " << y << ", z = " << z << std::endl;
	//the outpus is -> x = alpha, y = 10,z = 50.1

	
	

	return 0;
}