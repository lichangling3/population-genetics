#include "Population.h"

Population::Population(const Alleles& al): popAlleles(al){}


void Population::setFrequencies(std::vector<double> new_frequency)
{
	int i(0);
	if (new_frequency.size() == getSize()) {
		for (auto allele : popAlleles) {
			allele.second = new_frequency[i];
			++i;
		}
	} else {
		std::cerr << "Error : parameter new_frequency doesn't have the right size N " << std::endl;
	}
}

std::vector<double> Population::getFrequencies()
{
	std::vector<double> frequencies;
	for (auto allele:popAlleles){
		frequencies.push_back(allele.second);
	}
	return frequencies;
}
