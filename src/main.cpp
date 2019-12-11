#include <vector>
#include <string>
#include <tclap/CmdLine.h>
#include <Simulation.h>
#include <random.h>
#include <Display.h>
#include <FastaReader.h>
#include <iostream>

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
  provide a fitness coefficient for each allele \b in the order you entered them.
  \verbatim 
  ./PopulationGenetic -T 10 -R 2 -N 100 -A 2 -f 0.5 -f 0.5 -S 0.5 -S 0.8
  \endverbatim
  
  When run with a Fasta file, the programm can also simulate natural selection. To add this phenomenon,
  provide a fitness coefficient for each allele  \b in the order you entered them. Mutations can also be introduced 
  when a Fasta file is provided. To produce them, provide a mutation rate and the corresponding marker for every site be mutated.
  The order of mutation rates should match that of the marks.The marks without mutation rates will not be mutated. The chances of being mutated
  to one nucleotide or another are proportional to a number delta if using the Kimura model (see below).
  \verbatim
  ./PopulationGenetic -T 10 -R 2 -F ../tests/test_for_retrieveData.fasta -m 3 -m 6 -m 9 -m 12 -M 0.3 -M 0.4 -s 3 -s 12 -d 2/3
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

		TCLAP::ValueArg<size_t> duration("T", "time", "Simulation's duration", false, 10, "size_t");
		cmd.add(duration);
		TCLAP::ValueArg<size_t> repeat("R", "repeat", "Number of repetitions of simulation", false, 2, "size_t");
		cmd.add(repeat);
		TCLAP::ValueArg<size_t> nsample("N", "population_size", "Number of individuals", false, 100, "size_t");
		cmd.add(nsample);
		TCLAP::ValueArg<size_t> number_alleles("A", "number_of_alleles", "Number of alleles", false, 2, "size_t");
		cmd.add(number_alleles);
		TCLAP::ValueArg<std::string> file_name("F", "file_name", "Name of the FASTA file", false, "../tests/test_for_retrieveData.fasta", "string");
		cmd.add(file_name);
		TCLAP::MultiArg<double> freq("f", "frequences", "Initial frequences of the alleles", false, "double");
		TCLAP::MultiArg<size_t> marks("m", "marks", "Sequence positions when FASTA file provided", false, "size_t");
		cmd.xorAdd(marks, freq);
		TCLAP::MultiArg<double> mu("M", "mutation", "Mutation rate (<=1), please specifiy mutation sites", false, "double");
		cmd.add(mu);
		TCLAP::MultiArg<size_t> mu_sites("s", "mutation_site", "Mutation sites (should be the same as marks)", false, "size_t");
		cmd.add(mu_sites);
		TCLAP::MultiArg<double> fit("S", "fitness_coeff_without_file", "Fitness coefficient for each allele (>0 is favourable, between -1 and 0 is unfavourable and 0 or -1 is lethal)", false, "double");
		cmd.add(fit);
		TCLAP::ValueArg<double> delta("d", "mutation_delta", "Delta for Kimura model(should be between 1/3 and 1)", false, 1 / 3, "double");
		cmd.add(delta);

		cmd.parse(argc, argv);

		if (!duration.isSet())
		{
			throw std::runtime_error("Simulation duration needed");
		}
		if (!repeat.isSet())
		{
			throw std::runtime_error("Number of repetitions of simulation needed");
		}

		std::vector<double> new_fit;
		std::vector<double> mutations;
		std::vector<size_t> mutation_sites;

		if (file_name.isSet())
		{
			if (!marks.isSet())
			{
				throw std::runtime_error("Provide marks, frequencies not needed");
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
				else
				{
					std::cout << "You will not have any mutations (programm still runs)." << std::endl;
				}
			}
			if (mu.isSet())
			{
				if (!mu_sites.isSet())
				{
					throw std::runtime_error("Please provided mutation sites.");
				}
				else if (mu.getValue().size() != (mu_sites.getValue().size()))
				{
					throw std::runtime_error("The number of mutation rates should matches the number of mutation sites");
				}
			}
			if (mu_sites.isSet() && mu.isSet())
			{
				bool match(false);
				for (size_t i(0); i < mu_sites.getValue().size(); ++i)
				{
					for (size_t j(0); j < marks.getValue().size(); ++j)
					{
						if (mu_sites.getValue()[i] == marks.getValue()[j])
						{
							match = true;
							break;
						}
					}
				}
				if (!match)
				{
					throw std::runtime_error("Mutation sites should match marks.");
				}
				else
				{
					for (auto site : mu_sites.getValue())
					{
						mutation_sites.push_back(site);
					}
					for (auto mu_coeff : mu) {
						mutations.push_back(mu_coeff);
					}
				}
			}
			if (fit.isSet())
			{
				if ((fit.getValue()).size() > FastaReader::retrieveData(marks.getValue(), file_name.getValue()).size())
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
				for (size_t i(0); i < FastaReader::retrieveData(marks.getValue(), file_name.getValue()).size(); ++i)
				{
					new_fit.push_back(0.0);
				}
			}
			if (delta.getValue() < 1 / 3 || delta.getValue() > 1.0)
			{
				throw std::runtime_error("Delta value should be between 1/3 and 1.");
			}
			if (!mu.isSet() && !mu_sites.isSet())
			{
				mutations.push_back(0.0);
				mutation_sites.push_back(0);
			}

			Simulation sim(file_name.getValue(), marks.getValue(), duration.getValue(), repeat.getValue(), new_fit, mutations, mutation_sites, delta.getValue());
			sim.run();
		}
		else if (!file_name.isSet())
		{
			if (marks.isSet())
			{
				throw std::runtime_error("Provide frequences, marks not needed");
			}
			else if (!freq.isSet())
			{
				throw std::runtime_error("Initial frequences of each allele needed");
			}
			else if (nsample.getValue() < number_alleles.getValue())
			{
				throw std::runtime_error("There are too many alleles compared to the size of population");
			}
			else if (number_alleles.getValue() != (freq.getValue()).size())
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
				if ((fit.getValue()).size() != number_alleles.getValue())
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
				for (size_t i(0); i < number_alleles.getValue(); ++i)
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
			Simulation sim(nsample.getValue(), duration.getValue(), number_alleles.getValue(), freq.getValue(), repeat.getValue(), new_fit);
			sim.run();
		}
		if (!fit.isSet())
		{
			std::cout << "You will not have natural selection (programm still runs)." << std::endl;
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
