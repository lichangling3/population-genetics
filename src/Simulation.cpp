#include "Simulation.h"
#include "random.h"

Simulation::Simulation(size_t pop_size_, size_t sim_duration_, size_t nb_alleles_, std::vector<double> nb_freq_, size_t repetitions_, std::vector<double> new_fit)
	: sim_duration(sim_duration_), repetitions(repetitions_)
{
	Alleles alleles(Population::setAlleles(nb_alleles_, nb_freq_));
	populations.reserve(repetitions);
	for (size_t i(0); i < repetitions; ++i)
	{
		Population pop(alleles, pop_size_, new_fit);
		populations.push_back(pop);
	}
	my_flow = std::ofstream("display.txt");
}

Simulation::Simulation(std::string file_name_, std::vector<size_t> nb_marks_, size_t sim_duration_, size_t repetitions_, std::vector<double> new_fit_, std::vector<double> mu, std::vector<size_t> sites, double delta_, bool isMutation_, Alleles alleles, double mu_default_)
	: sim_duration(sim_duration_), nb_marks(nb_marks_), delta(delta_), repetitions(repetitions_), isMutation(isMutation_)
{
	if(isMutation)
	{
		for (size_t i(0); i < nb_marks.size(); ++i)
		{
			marks_mu.push_back(std::make_pair(nb_marks[i], mu_default_));
		}
		for (size_t i(0); i < nb_marks.size(); ++i)
		{
			for (size_t j(0); j < sites.size(); ++j)
			{
				if(nb_marks[i] == sites[j])
				{
					marks_mu[i].second = mu[j];
				}
			}
		}
	}
	size_t size_(FastaReader::size(nb_marks, file_name_));

	populations.reserve(repetitions);
	for (size_t i(0); i < repetitions; ++i)
	{
		Population pop(alleles, size_, new_fit_);
		populations.push_back(pop);
	}
	my_flow = std::ofstream("display.txt");
}

void Simulation::run()
{
	if (my_flow.is_open())
	{
		size_t gen_counter = 0;
		for (size_t j(0); j < sim_duration + 1; ++j)
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
