#include "Display.h"
#include <iomanip>

Display::Display(){}

void displayGen( Population pop ){ 
	
Alleles alleles = pop.getpopAlleles();

std::ofstream wright{"Team_1/display.txt"}; //will be created if it doesn't exist

if(wright){	//did it open well? 
	
	for (int i(0); i < 5 ; ++i){ //LE SIMULATION PAS ENCORE CREE donc mettre du style Simulation.nombre_generations_total au lieu de 5 (c'était juste pour que ca compile)
		wright << pop.getGeneration()<<  std::setw(5);
	
		for ( Alleles::iterator it = alleles.begin(); it !=alleles.end(); ++it){
			wright << it->second<<"|";
		}
		wright << std::endl;
	}
	wright << std::setw(5);
	for(Alleles::iterator it = alleles.begin(); it !=alleles.end(); ++it){		
		wright << it->first<<"|"; 
		}

}
else { std::cerr << "error while opening text file"<< std::endl;}

}
