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

void Population::setFitness(std::vector<double> new_fit) {
	fitness = new_fit;
}

void Population::step() {
	_RNG = new RandomNumbers();
	std::vector<double> start, end;
	size_t i(0);
	for(std::map<std::string, double>::iterator it = popAlleles.begin(); it != popAlleles.end(); ++it) {
	start.push_back(it->second);
	}
	end = _RNG->multinomial(size,start, fitness);
	for(std::map<std::string, double>::iterator it = popAlleles.begin(); it != popAlleles.end(); ++it) {
		it->second = end[i];
		++i;
	}
}

void Population::mutation(std::vector<std::pair<size_t,double>> marks, double delta) {
	_RNG = new RandomNumbers();
	for(size_t i(0); i < marks.size(); ++i) {	
		for(auto all:popAlleles) {
			std::string all_mutated = all.first;
			if (all.second*size > 0) { 
				double nbr_mutations = _RNG->binomial(all.second*size, marks[i].second);
				for(size_t j(0); j < nbr_mutations ; ++j) {
					all_mutated[i] = modelMut(all.first[i], delta);
					size_t old_size(popAlleles.size());
					popAlleles[all_mutated] += (1./size);
					if(popAlleles.size() != old_size) {
						fitness.push_back(0.0);
					}
				}
				popAlleles[all.first] -= (nbr_mutations/size);
			}
		}
	}
}

char Population::modelMut(char base, double delta){
	char b1, b2, b3;
	double alea = _RNG->uniform_double(0,1);
	
	if (base == 'C')
		b1 = 'T', b2 = 'A', b3 = 'G';
	else if (base == 'T')
		b1 = 'C', b2 = 'A', b3 = 'G';
	else if (base == 'G')
		b1 = 'A', b2 = 'T', b3 = 'C';
	else if (base == 'A')
		b1 = 'G', b2 = 'T', b3 = 'C';	
				
	if(alea < delta)
		return b1;
	if(alea > delta and alea < (1. - (1.-delta)/2))
		return b2;
	else
		return b3;
}

void Population::setSize(size_t size_) {
	size = size_;
}

void Population::setPopAlleles(std::map<std::string, double> map) {
	popAlleles = map;
}
