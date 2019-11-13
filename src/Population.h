#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <map>

typedef std::map<std::string, double> Alleles; //each allele codon and it's frequency

class Population {
public :
	Population(const Alleles& al, int gen);
	
	int getSize()const{return popAlleles.size();};
	
	Alleles getpopAlleles()const{return popAlleles;};
	
	int getGeneration()const {return generation;};		
		
private :
	Alleles popAlleles;	
	int generation; 	//the generation we are in at the moment
};

#endif
