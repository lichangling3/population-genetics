#include <vector>
#include <Population.h>

class Simulation {

public :
Simulation();
Simulation(const Population& pop);
void update(std::vector<double> new_number_alleles);

private :

Population population;

};

extern Simulation* _SIM;
