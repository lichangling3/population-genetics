#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <map>

typedef std::map<std::string, double> Alleles; //each allele codon and it's frequency

class Population {

public :
	Population(const Alleles& al, const int& taille);
	
	
	int getSize() const {return N;};
	
	Alleles getpopAlleles() const {return popAlleles;};
	
private :
	Alleles popAlleles;	
	const int N;
	

};

#endif
