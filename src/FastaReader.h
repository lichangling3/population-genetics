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
	static std::map<std::string, double> freq_per_allele(std::vector<int> marks, const std::string file_to_read); //returns the alleles and their initial frequency, sets N
	static size_t size(std::vector<int> marks, const std::string file_to_read); //returns population size

	private:
	static std::vector<std::string> readFile(std::vector<int> marks, const std::string file_to_read); //reads file provided and returns all the combinations indicated by the marks
};

#endif //FASTAREADER_H
