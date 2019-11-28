#include "Simulation.h"
#include "random.h"

Simulation::Simulation(size_t pop_size_, size_t sim_duration_, size_t nb_alleles_, std::vector<double> nb_freq_, size_t repetitions_, std::vector<double> new_fit)
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
		population.setFitness(new_fit);
	}
}

Simulation::Simulation(std::string file_name_, std::vector<size_t> nb_marks_, size_t sim_duration_, size_t repetitions_, std::vector<double> new_fit, std::vector<double> mu)
: sim_duration(sim_duration_), nb_marks(nb_marks_), repetitions(repetitions_)
{
	for(size_t i(0); i < mu.size(); ++i) {
		marks_mu.push_back(std::make_pair(nb_marks[i],mu[i]));
	}
	for(size_t i(0); i < repetitions; ++i) {
		Population pop;
		populations.push_back(pop);
	}
	my_flow = std::ofstream("display.txt");
	for(auto& population:populations) {
		population.setSize(FastaReader::size(nb_marks, file_name_));
		population.setWithFile(FastaReader::retrieveData(nb_marks, file_name_));
		population.setFitness(new_fit);
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
				population.step();
				population.mutation(marks_mu);
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

