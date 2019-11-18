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
	
	size_t getSize()const{return popAlleles.size();};
	
	Alleles getpopAlleles()const{return popAlleles;};
	/*
	std::vector<double> getFrequencies();
	void setFrequencies(std::vector<double> new_frequency);
	*/
	void setAlleles(size_t nb_alleles, std::vector<double> nb_freq_);
	void setWithFile(Alleles popAlleles_);
	void step();
	void setSize(size_t size_);
			
private :
	Alleles popAlleles;	
	size_t size;
	
};

#endif
