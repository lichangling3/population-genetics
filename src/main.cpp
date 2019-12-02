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
   
  \n 
  \b Extensions \n 
  When run without Fasta file, the program can also simulate natural selection. To add this penomenon,
  provide a fitness coefficient for each allele.
  \verbatim 
  ./PopulationGenetic -T 10 -R 2 -N 100 -A 2 -f 0.5 -f 0.5 -S 0.5 -S 0.8
  \endverbatim
  
  When run with a Fasta file, the programm can also simulate natural selection. To add this penomenon,
  provide a fitness coefficient for each allele with its triplet. Mutations can also be introduced 
  when a Fasta file is provided. To produce them, provide a mutation rate and the triplet of each allele to be mutated.
  \verbatim
  ./PopulationGenetic -T 10 -R 2 -F ../tests/test_for_retrieveData.fasta -m 3 -m 6 -m 9 -m 12 -M ATG,0.3 -M CTA,0.6 -M GCA,0.2 -M TAG,0.3
  \endverbatim
  Mutations are either based on the Jukes-Cantor or on the Kimura model. \n
  [(Jukes-Cantor model of DNA substition)](https://www.megasoftware.net/web_help_7/hc_jukes_cantor_distance.htm) \n
  [(Kimura two parameter model of DNA substition)](http://www.dbbm.fiocruz.br/molbiol/dnadist.html)
  */
  

RandomNumbers *_RNG;

struct TclapPair {
	///vector of 2 strings used to check that the argument parsed is indeed a pair
	std::string parts[1]; 
	std::pair<std::string, double> val;
	
	TclapPair& operator=(const std::string &str) {
		std::istringstream iss(str);
		
		// checks that at least 2 arguments are given
		if (!(iss >> parts[0] >> parts[1])) {
			throw TCLAP::ArgParseException(str + " is not a pair");
		}
		
		
		// check that nucleotides are valid
		for (size_t i = 0; i < parts[0].size(); ++i) {
			if (!(parts[0][i] == 'A' || parts[0][i] == 'T' || parts[0][i] == 'G' || parts[0][i] == 'C')) {
				throw TCLAP::ArgParseException(parts[0] + " is not a valid allele (nucleotides are ATGC)");
			}
		}
		
		// check that second value is a double
		double d = 0.0;
		try {
			d= std::stod(parts[1]);
		} catch (const std::invalid_argument &e) {
			throw TCLAP::ArgParseException(parts[1] + " is not a double");
		}
		
		val = std::make_pair(parts[0], d);
		return *this;
	}
};

/// This allows to parse a pair made of a string and a double using TCLAP
namespace TCLAP {
	template<>
	struct ArgTraits<TclapPair> {
		typedef StringLike ValueCategory;
	};
}

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
		TCLAP::MultiArg<TclapPair> mu("M", "mutation", "Mutation rate (<=1)", false, "mutation pair");
		cmd.add(mu);
		TCLAP::MultiArg<TclapPair> fit_fasta("s", "fitness_coeff_with_file", "Fasta File: fitness coefficient for each allele, >0 is favourable, between -1 and 0 is unfavourable and 0 or -1 is lethal",false, "selection pair");
		cmd.add(fit_fasta);
		TCLAP::MultiArg <double> fit_no_fasta ("S", "fitness_coeff_without_file", "No Fasta File: fitness coefficient for each allele, >0 is favourable, between -1 and 0 is unfavourable and 0 or -1 is lethal",false, "double");
		cmd.add (fit_no_fasta);
		
		cmd.parse(argc, argv);

    
		if (!duration.isSet()) {
			throw std::runtime_error("Simulation duration needed");
		}
		if (!repeat.isSet()) {
			throw std::runtime_error("Number of repetitions of simulation needed");
		}

		std::vector<double> new_fit;
		std::vector<double> mutations;	

		//_RNG = new RandomNumbers();

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
			}else if ((mu.getValue()).size() > (marks.getValue().size())){
				throw std::runtime_error ("The number of mutation rates should not exceed the number of marks");
			}
			if (mu.isSet()){
				bool in_file (false);
				for (auto coeff: mu.getValue()){
						for (auto allele: FastaReader::retrieveData(marks.getValue(), file_name.getValue())){
							if (coeff.val.first == allele.first){
								in_file = true;
								break;
							}
						}
					}
				if(!in_file)
				throw std::runtime_error ("Mutation rates can only be defined for existing alleles");
				else{
					for (auto coeff: mu.getValue()){
					mutations.push_back(coeff.val.second);
					}
				}
			}
			if (fit_fasta.isSet()){
				if ((fit_fasta.getValue()).size() > FastaReader::retrieveData(marks.getValue(), file_name.getValue()).size()){
					throw std::runtime_error ("The number of fitness coefficients should not exceed the number of alleles");
				}else{
					bool in_file (false);
					for (auto coeff: fit_fasta.getValue()){
						for (auto allele: FastaReader::retrieveData(marks.getValue(), file_name.getValue())){
							if (coeff.val.first == allele.first){
								in_file = true;
								break;
							}
						}
					}
					if (!in_file)
					throw std::runtime_error ("Fitness coefficients can only be defined for existing alleles");	
				}
					for (auto coeff: fit_fasta.getValue()){
						if (coeff.val.second < -1){
							throw std::runtime_error ("The fitness coefficient must be at least -1");
					}else{
							new_fit.push_back(coeff.val.second);
					}
				}
			} else if(!fit_fasta.isSet()){
				for(size_t i(0); i<FastaReader::retrieveData(marks.getValue(), file_name.getValue()).size(); ++i) {
					new_fit.push_back(0.0);
				}
			}
			if (fit_no_fasta.isSet()){
				throw std::runtime_error ("Fitness coefficients should be provided with alleles");
			}
				
			Simulation sim(file_name.getValue(), marks.getValue(), duration.getValue(), repeat.getValue(), new_fit, mutations);
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
			if (fit_no_fasta.isSet()) {
				for (auto coeff: fit_no_fasta.getValue()){
					new_fit.push_back(coeff);
				}
				if((fit_no_fasta.getValue()).size() > number_alleles.getValue()){
					throw std::runtime_error("The number of fitness coefficients should not exceed the number of alleles");
				}else{
					for (auto coeff: fit_no_fasta.getValue()){
						if (coeff < -1){
							throw std::runtime_error ("The fitness coefficient must be at least -1");
						}
					}
				}
			} else if(!fit_no_fasta.isSet()){
				for(size_t i(0); i<number_alleles.getValue(); ++i) {
					new_fit.push_back(0.0);
				}
			}
			if (mu.isSet()){
				throw std::runtime_error ("Mutations only possible with Fasta file");
			}
			if (fit_fasta.isSet()){
				throw std::runtime_error("Fitness coefficients with allele names only possible when Fasta file provided.");
			}
			Simulation sim(nsample.getValue(), duration.getValue(), number_alleles.getValue(), freq.getValue(), repeat.getValue(), new_fit);
			sim.run();
		}
		if (!fit_fasta.isSet() && !fit_no_fasta.isSet()){
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

	return nerr;
	}
