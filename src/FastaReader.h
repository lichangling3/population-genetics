#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

#ifndef FASTAREADER_H
#define FASTAREADER_H

class FastaReader
{
	public:
	FastaReader();	//constructor without arguments
	FastaReader(std::vector<int> marks_, const std::string file_to_read_);	//constructor with arguments used essentialy for tests
	std::map<std::string, double> freq_per_allele(); //returns the alleles and their initial frequency, sets N
	int get_N() {return N;} //returns population size

	private:
	std::vector<std::string> readFile(); //reads file provided and returns all the combinations indicated by the marks
	std::vector<int> marks;		//I expect marks given as a vector
	const std::string file_to_read;	//I expect file_to_read given as a string
	int N;
};

#endif //FASTAREADER_H
