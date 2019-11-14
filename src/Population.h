#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <map>

typedef std::map<std::string, double> Alleles; //each allele codon and it's frequency

class Population {
public :
	Population(const Alleles& al);
	
	int getSize()const{return popAlleles.size();};
	
	Alleles getpopAlleles()const{return popAlleles;};
			
private :
	Alleles popAlleles;	
	
};

#endif
