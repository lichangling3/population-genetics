#include "Simulation.h"
#include "random.h"

Simulation::Simulation(const Population& pop) : population(pop) {}

void Simulation::update(std::vector<double>& new_frequency_alleles) 
{
	if (new_number_alleles.size() != population.getSize()) 
	{
		std::cerr<< "parameter new_number_allele doesn't have the right size" <<std::endl;
	} else {
	for (auto& nbr : new_number_alleles)
	{
		nbr *= new_number_alleles.size();
	}
	population.setFrequencies(_RNG -> multinomial(new_number_alleles));
	}
}
