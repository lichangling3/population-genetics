#include <vector>
#include <string>
#include <tclap/CmdLine.h>
#include <Simulation.h>
#include <random.h>
#include <Display.h>
#include <FastaReader.h>

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
    size_t pop_size = nsample.getValue();
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
	
	if (!file_name.isSet()) {	
		
		Simulation sim(pop_size, sim_duration, nb_alleles, nb_freq, repetitions);
		sim.run();
	}	
	else {
		Simulation sim(file, nb_marks, sim_duration, repetitions);
		sim.run();
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

