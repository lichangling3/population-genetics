#include "Population.h"
#include <iostream>
#include <fstream>



class Display{
public:
	Display();
	
	static void displayGen(Population pop, std::ofstream& write);
	static void DisplayAll(Population pop, std::ofstream& write);
	
};	
	
