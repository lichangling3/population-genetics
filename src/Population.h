#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <string>
#include <vector>
#include <map>

typedef std::map<std::string, double> Alleles; //each allele codon and it's frequency

class Population {
public :
	Population();
	Population(const Alleles& al);
	
	size_t getSize()const{return popAlleles.size();};
	
	Alleles getpopAlleles()const{return popAlleles;};
	std::vector<double> getFrequencies() const;
	void setpopAlleles(const std::vector<std::string>& name, const std::vector<double>& frequency);
	void setFrequencies(const std::vector<double>& new_frequency);
			
private :
	Alleles popAlleles;	
	
};



#endif
