#include "Display.h"

Display::Display(){}

void display_1_gen( Population pop ){ 
//faudrait voir un update dans simulation ou pop pour les générations 
//car elles sont pas mémorisées donc les écrire tout de suite

Alleles alleles = pop.getpopAlleles();

std::ofstream wright{"Team_1/display.txt"}; //will be created if it doesn't exist

if(wright){ //did it open well? 
	
	for ( Alleles::iterator it = alleles.begin(); it!=alleles.end(); ++it){
		wright << it->second<<"|";
	}
	wright << std::endl;
}

else { std::cerr << "error while opening text file"<< std::endl;}

}


//pour l'affichage des noms des noms des alleles:
/*
for(Alleles::iterator it = alleles.begin(); it!=alleles.end(); ++it){
		wright << it->first<<"|";
	}
*/
