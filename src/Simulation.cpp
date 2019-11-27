#include "Simulation.h"
#include "random.h"

Simulation::Simulation(size_t pop_size_, size_t sim_duration_, size_t nb_alleles_, std::vector<double> nb_freq_, size_t repetitions_)
:sim_duration(sim_duration_), repetitions(repetitions_)
{
	for(size_t i(0); i < repetitions; ++i) {
		Population pop;
		populations.push_back(pop);
	}
	my_flow = std::ofstream("display.txt");
	for(auto& population:populations) {
		population.setSize(pop_size_);
		population.setAlleles(nb_alleles_, nb_freq_);
	}
}

Simulation::Simulation(std::string file_name_, std::vector<size_t> nb_marks_, size_t sim_duration_, size_t repetitions_)
: nb_marks(nb_marks_), sim_duration(sim_duration_), repetitions(repetitions_)
{
	for(size_t i(0); i < repetitions; ++i) {
		Population pop;
		populations.push_back(pop);
	}
	my_flow = std::ofstream("display.txt");
	for(auto& population:populations) {
		population.setSize(FastaReader::size(nb_marks, file_name_));
		population.setWithFile(FastaReader::retrieveData(nb_marks, file_name_));
	}
}

void Simulation::run() {
	size_t gen_counter = 0;
	my_flow << std::left << std::setw(15);
		for(size_t j(0); j < sim_duration+1; ++j) {
			my_flow << gen_counter;
			++gen_counter;
			for (auto& population:populations) {
				Display::displayGen(population, my_flow);
				std::cout<<population.getAllelesSize()<<std::endl;
				population.step();
				population.mutation({std::make_pair(2,0.01), std::make_pair(4,0.01), std::make_pair(8,0.01)});
			}
		my_flow << std::left << std::setw(15);
		my_flow << std::endl;
	}
	my_flow << std::right << std::setw(18);
	for (auto population:populations) {
		Display::displayAll(population, my_flow);
	}
	my_flow.close();
}

