#ifndef DISPLAY_H
#define DISPLAY_H

#include "Population.h"
#include <iostream>
#include <fstream>



class Display{
public:
	Display();
	
	static void displayGen(Population pop, std::ofstream& write);
	static void displayAll(Population pop, std::ofstream& write);
	
};	
	
#endif
