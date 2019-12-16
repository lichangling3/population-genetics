#include "Simulation.h"
#include "random.h"

Simulation::Simulation(int pop_size_, int sim_duration_, int nb_alleles_, std::vector<double> nb_freq_, int repetitions_, std::vector<double> new_fit)
	: sim_duration(sim_duration_), repetitions(repetitions_)
{
	Alleles alleles(Population::setAlleles(nb_alleles_, nb_freq_));
	populations.reserve(repetitions);
	for (int i(0); i < repetitions; ++i)
	{
		Population pop(alleles, pop_size_, new_fit);
		populations.push_back(pop);
	}
	my_flow = std::ofstream("display.txt");
}

Simulation::Simulation(std::string file_name_, std::vector<int> nb_marks_, int sim_duration_, int repetitions_, std::vector<double> new_fit, std::vector<double> mu, std::vector<int> sites, double delta_, bool isMutation_, Alleles alleles)
	: sim_duration(sim_duration_), nb_marks(nb_marks_), delta(delta_), repetitions(repetitions_), isMutation(isMutation_)
{
	for (size_t i(0); i < sites.size(); ++i)
	{
		marks_mu.push_back(std::make_pair(sites[i], mu[i]));
	}
	size_t size_(FastaReader::size(nb_marks, file_name_));

	populations.reserve(repetitions);
	for (int i(0); i < repetitions; ++i)
	{
		Population pop(alleles, size_, new_fit);
		populations.push_back(pop);
	}
	my_flow = std::ofstream("display.txt");
}

void Simulation::run()
{
	if (my_flow.is_open())
	{
		size_t gen_counter = 0;
		for (int j(0); j < sim_duration + 1; ++j)
		{
			my_flow << gen_counter;
			++gen_counter;
			for (auto &population : populations)
			{
				Display::displayGen(population, my_flow);
				population.step();
				if (isMutation)
				{
					population.mutation(marks_mu, delta);
				}
			}
			my_flow << "\n";
		}
		for (auto population : populations)
		{
			Display::displayAll(population, my_flow);
		}
	}
	else
	{
		std::cerr << "error while opening text file" << std::endl;
	}
	my_flow.close();
}
