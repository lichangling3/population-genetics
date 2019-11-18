#include "Population.h"


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

void Population::setPopAlleles(std::map<std::string, double> map) {
	popAlleles = map;
}
