#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <vector>
#include <map>
#include "random.h"

typedef std::map<std::string, double> Alleles; //each allele codon and it's frequency

class Population {
public :
	Population(){};
	size_t getSize()const{return size;};
	size_t getAllelesSize() const {return popAlleles.size();};
	Alleles getpopAlleles()const{return popAlleles;};
	void setAlleles(size_t nb_alleles, std::vector<double> nb_freq_);
	void setWithFile(Alleles popAlleles_);
	void step();
	void setSize(size_t size_);
	void setPopAlleles(std::map<std::string, double> map);
			
private :
	Alleles popAlleles;	
	size_t size;
	
};

#endif
