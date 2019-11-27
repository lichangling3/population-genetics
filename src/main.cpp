#include <vector>
#include <string>
#include <tclap/CmdLine.h>
#include <Simulation.h>
#include <random.h>
#include <Display.h>
#include <FastaReader.h>

/*!
  \mainpage Genetic drift dynamics
  This is an implementation of the model of Wright-Fisher
  [(Wright-Fisher Models, Approximations, and Minimum Increments of Evolution, William H. Press)] (https://pdfs.semanticscholar.org/1699/94e73dacc2bc1c0855d83b0a049e19f6b17f.pdf)
  
  The prgramm can be used without Fasta file with typical command:
  \verbatim
  ./PopulationGenetic -T 10 -R 2 -N 100 -A 2 -f 0.5 -f 0.5
  \endverbatim
  which will simulate the time evolution of a Population of 100 indiviuals with 2 different alleles, 
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
   
  \b Extensions
  When run without Fasta file, the program can also simulate natural selection. To add this penomenon,
  provide a fitness coefficient for each allele.
  \verbatim 
  ./PopulationGenetic -T 10 -R 2 -N 100 -A 2 -f 0.5 -f 0.5 -s 0.5 -s 0.8
  \endverbatim
  
  When run with a Fasta file, the programm can also simulate natural selection. To add this penomenon,
  provide a fitness coefficient for each allele.Mutations can also be introduced 
  when a Fasta file is provided. To produce them, provide a mutation rate for each site
  \verbatim
  ./PopulationGenetic -T 10 -R 2 -F ../tests/test_for_retrieveData.fasta -m 3 -m 6 -m 9 -m 12 -M 0.3 -M 0.6 -M 0.2 -M 0.3
  \endverbatim
  */

RandomNumbers *_RNG;
Simulation* _SIM;

int main(int argc, char **argv) {
	int nerr = 0;	
	try {
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
		TCLAP::MultiArg <double> freq("f", "frequences", "Initial frequences of the alleles", false, "double");
		TCLAP::MultiArg <size_t> marks("m", "marks", "Sequence positions when FASTA file provided", false, "size_t");
		cmd.xorAdd(marks, freq);
		TCLAP::MultiArg <double> mu("M", "mutation_rate", "Mutation are for each marker", false, "double");
		cmd.add(mu);
		TCLAP::MultiArg <double> fit("s", "fitness_coeff", "Fitness coefficient for each allele", false, "double");
		cmd.add(fit);

		cmd.parse(argc, argv);

		_RNG = new RandomNumbers();
		if (file_name.isSet()) {
			if (!marks.isSet()) {
				throw std::runtime_error("Provide marks, frequencies not needed");
			} 
			else if (nsample.isSet()) {
				throw std::runtime_error("Population size not needed");
			}
			else if (freq.isSet()) {
				throw std::runtime_error("Frequences not needed");
			}
			else if (!marks.isSet()) {
				throw std::runtime_error("Marks needed");
			}
			if (!mu.isSet()){
				std::cout<<"You will not have any mutations."<<std::endl;
			}else if ((mu.getValue()).size() != (marks.getValue().size())){
				throw std::runtime_error ("The number of mutation rates should match the number of marks");
			}
			if (fit.isSet()){
				if ((fit.getValue()).size() != FastaReader::size(marks.getValue(), file_name.getValue())){
					throw std::runtime_error ("The number of fitness coefficients should match the number of alleles");
				}else{
					for (auto coeff: fit.getValue()){
						if (coeff < -1){
							throw std::runtime_error ("The fitness coefficient must be at least -1");
						}
					}
				}
			}
			Simulation sim(file_name.getValue(), marks.getValue(), duration.getValue(), repeat.getValue());
			sim.run();
			
		} else if (!file_name.isSet()) {
			if (marks.isSet()) {
				throw std::runtime_error("Provide frequences, marks not needed");
			}
			else if (!freq.isSet()) {
				throw std::runtime_error("Initial frequences of each allele needed");
			}
			else if (nsample.getValue() < number_alleles.getValue()) {
				throw std::runtime_error("There are too many alleles compared to the size of population");
			}
			else if (number_alleles.getValue() != (freq.getValue()).size()) {
				throw std::runtime_error("The number of alleles must be equal to the number of frequences provided");
			} else {
				double sum(0);
				for (size_t i(0); i < (freq.getValue()).size(); ++i) {
					sum += (freq.getValue())[i];
				}
				if (std::abs(sum - 1.0) > 1e-6) {
					throw std::runtime_error("The sum of frequences must be equal to 1.0");
				}
			}
			if (fit.isSet() && (fit.getValue()).size() != number_alleles.getValue()){
				throw std::runtime_error("The number of fitness coefficients should match the number of alleles");
			}else{
					for (auto coeff: fit.getValue()){
						if (coeff < -1){
							throw std::runtime_error ("The fitness coefficient must be at least -1");
						}
					}
				}
			if (mu.isSet()){
				throw std::runtime_error ("Mutations only possible with Fasta file");
			}
			Simulation sim(nsample.getValue(), duration.getValue(), number_alleles.getValue(), freq.getValue(), repeat.getValue());
			sim.run();
		}
			
		if (!fit.isSet()){
			std::cout<<"You will not have natural selection."<<std::endl;
		}
		
	} catch(std::runtime_error &e) {
		std::cerr << e.what() << "\n";
		nerr = 1;
	} catch(TCLAP::ArgException &e) {
        std::cerr << "Error: " + e.error() + " " + e.argId();
       	nerr = 2;
    }
    if (_RNG) delete _RNG;
    if (_SIM) delete _SIM;
	return nerr;
}
