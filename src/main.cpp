#include <vector>
#include <string>
#include <tclap/CmdLine.h>
#include <Simulation.h>
#include <random.h>
#include <Display.h>
#include <FastaReader.h>

// void parse_args(int argc, char **argv) 
// {
// 	TCLAP::CmdLine cmd("Population definition");

// 	TCLAP::ValueArg<size_t> nsample("N", "population_size", "Number of individuals", false, 100, "size_t");
//     cmd.add(nsample);
// 	TCLAP::ValueArg<size_t> number_alleles("A", "number_of_alleles", "Number of alleles", false, 2, "size_t");
//     cmd.add(number_alleles);
//     TCLAP::ValueArg<int> duration("T", "time", "Simulation's duration", false, 10, "size_t");
//     cmd.add(duration);
//     TCLAP::ValueArg<int> repeat("R", "repeat", "Number of repetitions of simulation", false, 2, "size_t");
//     cmd.add(repeat);
// 	TCLAP::ValueArg<std::string> file_name("F", "file_name", "Name of the FASTA file", false, "../tests/test_for_retrieveData.fasta", "string");
//     cmd.add(file_name);
// 	TCLAP::MultiArg<double> freq("f", "frequences", "Initial frequences of the alleles", false, "double");
// 	TCLAP::MultiArg<size_t> marks("m", "marks", "Sequence positions when FASTA file provided", false, "size_t");
// 	cmd.xorAdd(marks, freq);

//     cmd.parse(argc, argv);
    
// 	if (!duration.isSet()) {
// 		throw std::runtime_error("Simulation duration needed");
// 	}
// 	if (!repeat.isSet()) {
// 		throw std::runtime_error("Number of repetitions of simulation needed");
// 	}
// 	int time(duration.getValue());
// 	int repetition(repeat.getValue());
    
// 	if (file_name.isSet()) {
// 		if (!marks.isSet()) {
// 			throw std::runtime_error("Provide marks, frequences not needed");
// 		} 
// 		else if (nsample.isSet()) {
// 			throw std::runtime_error("Population size not needed");
// 		}
// 		else if (freq.isSet()) {
// 			throw std::runtime_error("Frequences not needed");
// 		}
// 		else if (!marks.isSet()) {
// 			throw std::runtime_error("Marks needed");
// 		}
// 		std::map<std::string, double> freq_per_allele(FastaReader::retrieveData(marks.getValue(), file_name.getValue()));
// 		int population_size(FastaReader::size(marks.getValue(), file_name.getValue()));
// 	} else if (!file_name.isSet()) {
// 		if (marks.isSet()) {
// 			throw std::runtime_error("Provide frequences, marks not needed");
// 		}
// 		else if (!nsample.isSet()) {
// 			throw std::runtime_error("Number of individuals needed");
// 		}
// 		else if (!number_alleles.isSet()) {
// 			throw std::runtime_error("Number of alleles needed");
// 		}
// 		else if (!freq.isSet()) {
// 			throw std::runtime_error("Initial frequences of each allele needed");
// 		}
// 		else if (nsample.getValue() < number_alleles.getValue()) {
// 			throw std::runtime_error("There is too much alleles compared to the size of population");
// 		}
// 		else if (number_alleles.getValue() != (freq.getValue()).size()) {
// 			throw std::runtime_error("The number of alleles must be equal to the number of frequences provided");
// 		} else {
// 			double sum(0);
// 			for (size_t i(0); i < (freq.getValue()).size(); ++i) {
// 				sum += (freq.getValue())[i];
// 			}
// 			if (std::abs(sum - 1.0) > 1e-6) {
// 				throw std::runtime_error("The sum of frequences must be equal to 1.0");
// 			}
// 		}
// 		int population_size(nsample.getValue());
// 		int nb_alleles(number_alleles.getValue());
// 		std::vector<double> frequences(freq.getValue());
// 	}
// }

// int main(int argc, char **argv) 
// {
// 	int nerr = 0;
// 	try {
// 		//Petit résumé des valeurs à manipuler
// 		// int time
// 		// int repetion
// 		// std::map<std::string, double> freq_per_allele (lorsque l'utilisateur fournit un fichier FASTA)
// 		// int population_size
// 		// int nb_alleles
// 		// std::vector<double> frequences

// 		//Mettre ici vos fonctions pour initialiser simulation, etc.
// 	} catch(std::runtime_error &e) {
// 		std::cerr << e.what() << "\n";
// 		nerr = 1;
// 	} catch(TCLAP::ArgException &e) {
//         std::cerr << "Error: " + e.error() + " " + e.argId();
//         nerr = 2;
//     }
// 	return nerr;
// }

RandomNumbers *_RNG;
Simulation* _SIM;

int main(int argc, char **argv) 
{
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

    cmd.parse(argc, argv);
    
    //retrieve values maybe needed for simulation constructor
    size_t sim_duration = duration.getValue();
    size_t repetitions = repeat.getValue();
    size_t nb_alleles = number_alleles.getValue();
    std::string file = file_name.getValue();
    std::vector<double> nb_freq = freq.getValue();
    std::vector<size_t> nb_marks = marks.getValue();
    
	if (file_name.isSet()) {
		if (!marks.isSet()) {
			throw std::runtime_error("Provide marks, frequences not needed");
		} 
		if (nsample.isSet()) {
			throw std::runtime_error("Population size not needed");
		}
		if (freq.isSet()) {
			throw std::runtime_error("Frequences not needed");
		}
	} else if (!file_name.isSet()) {
		if (marks.isSet()) {
			throw std::runtime_error("Marks not needed");
		}
		 if (nb_freq.size() < number_alleles.getValue()){
			 throw std::runtime_error("Initial frequences needed for all alleles");
		 }
		 else if (nb_freq.size() > number_alleles.getValue()){
			 throw std::runtime_error("Number of frequences should be equal to number of alleles");
		 }
	}
	
	
	
	_RNG = new RandomNumbers();
	
	Display display;
	FastaReader fasta;
	if (file_name.isSet()) {	
		
		Alleles alleles = fasta.retrieveData(nb_marks, file);
		Population pop(alleles);
		_SIM = new Simulation(pop);
		display.displayGen(pop, sim_duration);
	//~ parse_args(argc, argv);
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

