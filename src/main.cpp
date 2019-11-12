#include <vector>
#include <string>
#include <tclap/CmdLine.h>

//Global variables for FastaReader
std::vector<int> _MARKS_;
std::string _FILE_TO_READ_;

void parse_args(int argc, char **argv) 
{
	TCLAP::CmdLine cmd("Population definition");

    TCLAP::ValueArg<int> duration("T", "time", "Simulation's duration", false, 10, "int");
    cmd.add(duration);
    TCLAP::ValueArg<int> repeat("R", "repeat", "Number of repetitions of simulation", false, 2, "int");
    cmd.add(repeat);
	TCLAP::ValueArg<int> nsample("N", "population_size", "Number of individuals", false, 100, "int");
    cmd.add(nsample);
    TCLAP::ValueArg<int> number_alleles("A", "number_of_alleles", "Number of alleles", false, 2, "int");
    cmd.add(number_alleles);
	TCLAP::ValueArg<std::string> file_name("F", "file_name", "Name of the FASTA file", false, "../tests/test_for_retrieveData.fasta", "string");
    cmd.add(file_name);
	TCLAP::UnlabeledMultiArg<size_t> marks("marks", "Sequence positions when FASTA file provided", false, "size_t");
	TCLAP::UnlabeledMultiArg<double> freq("frequences", "Initial frequences of the alleles", false, "double");
	cmd.xorAdd(marks, freq);

    cmd.parse(argc, argv);

	if (file_name.isSet()) {
		std::vector<double> vec = freq.getValue();
		for (size_t i(0); i < vec.size(); ++i) {
			std::cout << vec[i] << " " << std::endl;
		}
		if (!marks.isSet()) {
			throw std::runtime_error("Provide marks");
		} 
		if (nsample.isSet()) {
			throw std::runtime_error("Population size not needed");
		}
		if (freq.isSet()) {
			throw std::runtime_error("Frequences not needed");
		}
	} else if (!file_name.isSet()) {
		if (!freq.isSet()) {
			throw std::runtime_error("Initial frequences needed");
		}
		if (marks.isSet()) {
			throw std::runtime_error("Marks not needed");
		}
	}
}

int main(int argc, char **argv) 
{
	int nerr = 0;
	try {
		parse_args(argc, argv);
		
		//Mettre ici vos fonctions pour initialiser simulation.
	} catch(std::runtime_error &e) {
		std::cerr << e.what() << "\n";
		nerr = 1;
	} catch(TCLAP::ArgException &e) {
        std::cerr << "Error: " + e.error() + " " + e.argId();
        nerr = 2;
    }
	return nerr;
}

