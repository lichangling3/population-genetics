#include <vector>
#include <string>
#include <tclap/CmdLine.h>
#include <Simulation.h>
#include <random.h>
#include <Display.h>
#include <FastaReader.h>
#include <iostream>
#include <algorithm>

/*!
  \mainpage Genetic drift dynamics
  This is an implementation of the model of Wright-Fisher
  [(Wright-Fisher Models, Approximations, and Minimum Increments of Evolution, William H. Press)] (https://pdfs.semanticscholar.org/1699/94e73dacc2bc1c0855d83b0a049e19f6b17f.pdf)
  
  The program can be used without Fasta file with typical command:
  \verbatim
  ./PopulationGenetic -T 10 -R 2 -N 100 -A 2 -f 0.5 -f 0.5
  \endverbatim
  which will simulate the time evolution of a Population of 100 individuals with 2 different alleles, 
  both with frequency 0.5.
  The Simulation will last 10 seconds and be repeated 2 times.
  
  or with a Fasta file with typical command:
  \verbatim
  ./PopulationGenetic -T 10 -R 2 -F ../tests/test_for_retrieveData.fasta -m 3 -m 6 -m 9 -m 12
  \endverbatim
  which will simulate the time evolution of the Population described in the Fasta file. 
  The alleles will be found thanks to the provided marks.
  The Simulation will last 10 seconds and be repeated 2 times.
  The generated output will by a table of the alleles' frequencies for each generation of each simulation.
  The corresponding alleles can be found at the bottom of the corresponding column.
   
  \n 
  \b Extensions \n 
  When run without Fasta file, the program can also simulate natural selection. To add this phenomenon,
  provide a fitness coefficient for each allele in the order you entered them.
  \verbatim 
  ./PopulationGenetic -T 10 -R 2 -N 100 -A 2 -f 0.5 -f 0.5 -S 0.5 -S 0.8
  \endverbatim
  
  When run with a Fasta file, the programm can also simulate natural selection. To add this phenomenon,
  provide a fitness coefficient for each allele in the order you entered them. Mutations can also be
  introduced when a Fasta file is provided. Two different ways of producing mutations are possible: \n
  - either enter one default mutation rate that is the same for every marker \n
  - or provide a mutation rate and the corresponding marker for every site be mutated, as well as a default
   mutation rate that will be used to mutate all other sites (or not, if the default rate is 0). The order 
   of mutation rates should match that of the marks. \n
   \n
  The chances of being mutated to one nucleotide or another are proportional to a number delta if using the Kimura model (see below).
  \verbatim
  ./PopulationGenetic -T 10 -R 2 -F ../tests/test_for_retrieveData.fasta -m 3 -m 6 -m 9 -m 12 -M 0.3 -M 0.4 -s 3 -s 12 -d 2/3 -D 0.00001
  \endverbatim
  Mutations are either based on the Jukes-Cantor or on the Kimura model. \n
  [(Jukes-Cantor model of DNA substition)](https://www.megasoftware.net/web_help_7/hc_jukes_cantor_distance.htm) \n
  [(Kimura two parameter model of DNA substition)](http://www.dbbm.fiocruz.br/molbiol/dnadist.html)
  */

RandomNumbers *_RNG;

int main(int argc, char **argv)
{
	int nerr = 0;
	_RNG = new RandomNumbers();
	try
	{
		TCLAP::CmdLine cmd("Population definition");

		TCLAP::ValueArg<int> duration("T", "time", "Simulation's duration", false, 10, "size_t");
		cmd.add(duration);
		TCLAP::ValueArg<int> repeat("R", "repeat", "Number of repetitions of simulation", false, 2, "size_t");
		cmd.add(repeat);
		TCLAP::ValueArg<int> nsample("N", "population_size", "Number of individuals", false, 100, "size_t");
		cmd.add(nsample);
		TCLAP::ValueArg<int> number_alleles("A", "number_of_alleles", "Number of alleles", false, 2, "size_t");
		cmd.add(number_alleles);
		TCLAP::ValueArg<std::string> file_name("F", "file_name", "Name of the FASTA file", false, "../tests/test_for_retrieveData.fasta", "string");
		cmd.add(file_name);
		TCLAP::MultiArg<double> freq("f", "frequences", "Initial frequences of the alleles", false, "double");
		TCLAP::MultiArg<int> marks("m", "marks", "Sequence positions when FASTA file provided", false, "size_t");
		cmd.xorAdd(marks, freq);
		TCLAP::MultiArg<double> mu("M", "mutation", "Mutation rate (<=1.0), please specifiy mutation sites", false, "double");
		cmd.add(mu);
		TCLAP::MultiArg<int> mu_sites("s", "mutation_site", "Mutation sites (should be the same as marks)", false, "size_t");
		cmd.add(mu_sites);
		TCLAP::MultiArg<double> fit("S", "fitness_coeff_without_file", "Fitness coefficient for each allele (>0 is favourable, between -1 and 0 is unfavourable and 0 or -1 is lethal)", false, "double");
		cmd.add(fit);
		TCLAP::ValueArg<double> delta("d", "mutation_delta", "Delta for Kimura model(should be between 1/3 and 1)", false, 1. / 3, "double");
		cmd.add(delta);
		TCLAP::ValueArg<double> mu_default("D", "default_rate", "Mutation rate by default (should be between 0.0 and 1.0)", false, 0.0, "double");
		cmd.add(mu_default);

		cmd.parse(argc, argv);

		if (!duration.isSet())
		{
			throw std::runtime_error("Simulation duration needed");
		}
		else if (duration.isSet() && duration.getValue() <= 0)
		{
			throw std::runtime_error("Simulation's duration should be positive");
		}
		if (!repeat.isSet())
		{
			throw std::runtime_error("Number of repetitions of simulation needed");
		}
		else if (repeat.isSet() && repeat.getValue() < 0)
		{
			throw std::runtime_error("Number of repetitions should be positive");
		}

		std::vector<double> new_fit;
		std::vector<double> mutations;
		std::vector<int> mutation_sites;
		bool isMutation(false);

		if (file_name.isSet())
		{
			if (!marks.isSet())
			{
				throw std::runtime_error("Provide marks");
			}
			else if (marks.isSet())
			{
				for (auto site : marks)
				{
					if (site <= 0.0)
					{
						throw std::runtime_error("Marks should be positive");
					}
				}
			}
			else if (nsample.isSet())
			{
				throw std::runtime_error("Population size not needed");
			}
			else if (freq.isSet())
			{
				throw std::runtime_error("Frequences not needed");
			}
			else if (!marks.isSet())
			{
				throw std::runtime_error("Marks needed");
			}
			if (!mu.isSet())
			{
				if (mu_sites.isSet())
				{
					throw std::runtime_error("Please set mutation rates for provided mutation sites");
				}
				else if (!mu_default.isSet())
				{
					std::cout << "You will not have any mutations." << std::endl;
				}
			}
			if (mu.isSet())
			{
				if (!mu_sites.isSet())
				{
					throw std::runtime_error("Please provided mutation sites.");
				}
				else if (!mu_default.isSet())
				{
					throw std::runtime_error("Please set a default mutation rate");
				}
				else if (mu.getValue().size() != (mu_sites.getValue().size()))
				{
					throw std::runtime_error("The number of mutation rates should matches the number of mutation sites");
				}
				for (auto mu_coeff : mu.getValue())
				{
					if (mu_coeff < 0.0)
					{
						throw std::runtime_error("Mutation rates should be positive");
					}
					else if (mu_coeff > 1.0)
					{
						throw std::runtime_error("Mutation rates should not be bigger than 1");
					}
				}
			}

			if (mu_default.isSet())
			{
				if (mu_default.getValue() < 0)
				{
					throw std::runtime_error("Default mutation rate should be positive");
				}
				else if (mu_default.getValue() > 1)
				{
					throw std::runtime_error("Default mutation rate should not exceed 1");
				}
			}

			if (mu_sites.isSet() && mu.isSet() && mu_default.isSet())
			{
				for (auto site : mu_sites.getValue())
				{
					mutation_sites.push_back(site);
				}
				for (auto mu_coeff : mu)
				{
					mutations.push_back(mu_coeff);
				}
				isMutation = true;
				for (size_t i(0); i < mu_sites.getValue().size(); ++i)
				{
					if (std::find(marks.getValue().begin(), marks.getValue().end(), mutation_sites[i]) != marks.getValue().end())
					{
						continue;
					}
					else
					{
						throw std::runtime_error("Mutation sites should be contained in the marks sites");
					}
				}
			}

			if (mu_default.isSet())
			{
				isMutation = true;
			}

			std::map<std::string, double> retrieveAlleles = FastaReader::retrieveData(marks.getValue(), file_name.getValue());

			if (fit.isSet())
			{
				if ((fit.getValue()).size() > retrieveAlleles.size())
				{
					throw std::runtime_error("The number of fitness coefficients should not exceed the number of alleles");
				}
				for (auto coeff : fit.getValue())
				{
					if (coeff < -1)
					{
						throw std::runtime_error("The fitness coefficient must be at least -1");
					}
					else
					{
						new_fit.push_back(coeff);
					}
				}
			}
			else if (!fit.isSet())
			{
				for (size_t i(0); i < retrieveAlleles.size(); ++i)
				{
					new_fit.push_back(0.0);
				}
			}
			if (delta.getValue() < 1 / 3 || delta.getValue() > 1.0)
			{
				throw std::runtime_error("Delta value should be between 1/3 and 1.");
			}
			if (!mu.isSet() && !mu_sites.isSet() && !mu_default.isSet())
			{
				mutations.push_back(0.0);
				mutation_sites.push_back(0);
			}
			if (!fit.isSet())
			{
				std::cout << "You will not have natural selection."<< std::endl;
			}
			Simulation sim(file_name.getValue(), marks.getValue(), duration.getValue(), repeat.getValue(), new_fit, mutations, mutation_sites, delta.getValue(), isMutation, retrieveAlleles, mu_default.getValue());
			sim.run();
		}
		else if (!file_name.isSet())
		{
			int freq_size = freq.getValue().size();
			if (marks.isSet())
			{
				throw std::runtime_error("Provide frequences, marks not needed");
			}
			else if (!freq.isSet())
			{
				throw std::runtime_error("Initial frequences of each allele needed");
			}
			else if (freq.isSet())
			{ for (auto element : freq)
				{
					if (element < 0.0 || element > 1.0)
					{
						throw std::runtime_error ("Frequencies should be positive and not bigger than 1");
					}
				}
			}
			if (nsample.getValue() < 0)
			{
				throw std::runtime_error("Sample size should be positive");
			}
			else if (nsample.getValue() < number_alleles.getValue())
			{
				throw std::runtime_error("There are too many alleles compared to the size of population");
			}
			if (number_alleles.getValue() < 0)
			{
				throw std::runtime_error("The number of alleles should be positive");
			}
			else if (number_alleles.getValue() != freq_size)
			{
				throw std::runtime_error("The number of alleles must be equal to the number of frequences provided");
			}
			else
			{
				double sum(0);
				for (size_t i(0); i < (freq.getValue()).size(); ++i)
				{
					sum += (freq.getValue())[i];
				}
				if (std::abs(sum - 1.0) > 1e-6)
				{
					throw std::runtime_error("The sum of frequences must be equal to 1.0");
				}
			}
			if (fit.isSet())
			{
				for (auto coeff : fit.getValue())
				{
					new_fit.push_back(coeff);
				}
				size_t nb_alleles = number_alleles.getValue();
				if ((fit.getValue()).size() != nb_alleles)
				{
					throw std::runtime_error("The number of fitness coefficients should match the number of alleles");
				}
				else
				{
					for (auto coeff : fit.getValue())
					{
						if (coeff < -1)
						{
							throw std::runtime_error("The fitness coefficient must be at least -1");
						}
					}
				}
			}
			else if (!fit.isSet())
			{
				for (int i(0); i < number_alleles.getValue(); ++i)
				{
					new_fit.push_back(0.0);
				}
			}
			if (mu.isSet())
			{
				throw std::runtime_error("Mutations only possible with Fasta file");
			}
			if (delta.isSet())
			{
				throw std::runtime_error("Mutations only possible with Fasta file");
			}
			if (!fit.isSet())
			{
				std::cout << "You will not have natural selection."<< std::endl;
			}
			Simulation sim(nsample.getValue(), duration.getValue(), number_alleles.getValue(), freq.getValue(), repeat.getValue(), new_fit);
			sim.run();
		}
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << "\n";
		nerr = 1;
	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "Error: " + e.error() + " " + e.argId();
		nerr = 2;
	}
	if (_RNG)
		delete _RNG;
	return nerr;
}
