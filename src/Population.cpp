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
	end = _RNG->multinomial(size,start);
	for(std::map<std::string, double>::iterator it = popAlleles.begin(); it != popAlleles.end(); ++it) {
		it->second = end[i];
		++i;
	}
}

void Population::mutation(std::vector<std::pair<size_t,double>> marks) {
	_RNG = new RandomNumbers();
	for(size_t i(0); i < marks.size(); ++i)
	{	
		for(auto all:popAlleles) {
		
		std::string all_mutated = all.first;
		
		
		if (all.second*size > 0){ 
		
		
		double nbr_mutations = _RNG->binomial(all.second*size, marks[i].second);
		
		
		for(size_t j(0); j < nbr_mutations ; ++j){
			all_mutated[i] = single_base_mut(all.first[i]);
			popAlleles[all_mutated] += (1./size);
			
			
		}
		
		//popAlleles[all_mutated] += (nbr_mutations/size * all.second);
		popAlleles[all.first] -= (nbr_mutations/size);
	}
		
			/*std::vector<char> nucl{'A','C','T','G'};
			for(size_t j(0); j < nucl.size(); ++j)
			{
				if(all.first[i] != nucl[j])
				if(alea<1/3){ all.first[i] = ;}
				if (alea>1/3 and alea<2/3){ all.first[i] = "T";}
				if(alea >2/3 ){ all.first[i] = "G";}
				}
			}*/
		}
		/*std::map<std::string,double>::iterator it;
		it = popAlleles.find(all_mutated);
		if(it != popAlleles.end()){
			popAlleles[all.first] -= 1/size;
			}
		else {
			popAlleles[all_mutated] += 1/size;
			popAlleles[all.first] -= 1/size;
		}*/
		}
		
		
}



char Population::single_base_mut(char base){
	char b1, b2, b3;
	double alea = _RNG->uniform_double(0,1);
	
	if (base == 'C'){
		b1 = 'A', b2 = 'T', b3 = 'G';}
		
	if (base == 'T'){
		b1 = 'A', b2 = 'C', b3 = 'G';}
		
	if (base == 'G'){
		b1 = 'A', b2 = 'T', b3 = 'C';}
		
	if (base == 'A'){
		b1 = 'C', b2 = 'T', b3 = 'G';}	
				
	if(alea<(1/3)){return b1;}
	if(alea>(1/3) and alea<(2/3)){ return b2;}
	else{ return b3;}
}



void Population::setSize(size_t size_) {
	size = size_;
}

void Population::setPopAlleles(std::map<std::string, double> map) {
	popAlleles = map;
}
