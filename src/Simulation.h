#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <Population.h>

class Simulation {

public :
Simulation();
Simulation(const Population& pop);
void update(std::vector<double>& new_frequency_alleles);

private :

Population population;

};

extern Simulation* _SIM;

#endif
