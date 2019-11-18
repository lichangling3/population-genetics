#include "Population.h"


/*
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
*/
void Population::setAlleles(size_t nb_alleles, std::vector<double> nb_freq_)
{
	for(size_t i(0); i < nb_alleles; ++i) {
		std::string key = std::to_string(i+1);
		popAlleles[key] = nb_freq_[i];
	}
}

void Population::setWithFile(Alleles popAlleles_) {
	popAlleles = popAlleles_;
}

void Population::step() {
	_RNG = new RandomNumbers();
	std::vector<double> start, end;
	size_t i(0);
	for(std::map<std::string, double>::iterator it = popAlleles.begin(); it != popAlleles.end(); ++it) {
	start.push_back(it->second);
	}
	end = _RNG->multibinomial(size,start);
	for(std::map<std::string, double>::iterator it = popAlleles.begin(); it != popAlleles.end(); ++it) {
		it->second = end[i];
		++i;
	}
}

void Population::setSize(size_t size_) {
	size = size_;
}

