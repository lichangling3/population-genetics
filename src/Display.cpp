#include "Display.h"
#include "Simulation.h"
#include <iomanip>

Display::Display(){}

void displayGen(Population pop , size_t time){ 
	

std::ofstream write;
write.open("Team_1/display.txt"); //will be created if it doesn't exist

if(write){	//did it open well? 
	
	for (int i(0); i < time ; ++i){ //LE SIMULATION PAS ENCORE CREE donc mettre du style Simulation.nombre_generations_total au lieu de 5 (c'était juste pour que ca compile)
		write << i <<  std::setw(5);
	
		for ( Alleles::iterator it = pop.getpopAlleles().begin(); it !=pop.getpopAlleles().end(); ++it){
			write << it->second<<"|";
		}
		_SIM -> update(pop.getFrequencies());
		write << std::endl;
	}
	write << std::setw(5);
	for(Alleles::iterator it = pop.getpopAlleles().begin(); it !=pop.getpopAlleles().end(); ++it){		
		write << it->first<<"|"; 
		}

}
else { std::cerr << "error while opening text file"<< std::endl;}

}
