#include "Display.h"
#include "Simulation.h"
#include <iomanip>

Display::Display(){}


void Display::displayGen(Population pop , std::ofstream& write){ 
 //will be created if it doesn't exist

	if(write){	//did it open well? 
		for (auto all:pop.getpopAlleles()) {
			write << all.second <<"|";
		}
		write << std::endl;
	}
	else { 
		std::cerr << "error while opening text file"<< std::endl;
	}
}

void Display::DisplayAll(Population pop, std::ofstream& write) {
	if(write){
		for(auto all:pop.getpopAlleles()){		
			write << all.first <<"|"; 
		}
	}
	else {
		std::cerr << "error while opening text file"<< std::endl;
	}
}
