#include <vector>
#include <Population.h>
#include <fstream>
#include <FastaReader.h>
#include <Display.h>
#include <iomanip>


class Simulation {

public :
Simulation(size_t pop_size_, size_t sim_duration_, size_t nb_alleles_, std::vector<double> nb_freq_, size_t repetitions_);
Simulation(std::string file_name_, std::vector<size_t> nb_marks_, size_t sim_suration_, size_t repetitions_);
/*Simulation(const Population& pop);
void update(std::vector<double> new_number_alleles);*/
void run();

private :

std::vector<Population> populations;
size_t sim_duration;
size_t pop_size;
size_t repetitions;
std::ofstream my_flow;
static size_t gen_counter;

};

extern Simulation* _SIM;
