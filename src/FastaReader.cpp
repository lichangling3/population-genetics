#include "FastaReader.h"

FastaReader::FastaReader()
:marks(_MARKS_), file_to_read(_FILE_TO_READ_);		//I expect _MARKS_ and _FILE_TO_READ_ as global elements
{}

FastaReader::FastaReader(std::vector<int> marks_, const std::string file_to_read_)
:marks(marks_), file_to_read(file_to_read_)
{
    std::sort(marks.begin(), marks.end());
}

std::vector<std::string> FastaReader::readFile() 
{
    std::vector<std::string> all_combinations;
    try {
        std::ifstream confstr(file_to_read);
        if (confstr.is_open()) {
            std::string line, id, genome, combination;
            while(std::getline(confstr, line )) {
                if(line.empty() or line[0] == '>' ) { 
                    if(!id.empty()) { 
                        id.clear();
                    }
                    if(!line.empty()) {
                        id = line.substr(1);
                    }
                    genome.clear();
                } else if(!id.empty()) {
                    genome += line;
                    // This removes all spaces from the string
                    genome.erase(std::remove_if(genome.begin(), genome.end(), isspace), genome.end());
                    // This puts all the letters in upper case
                    std::transform(genome.begin(), genome.end(), genome.begin(), ::toupper);
                    for (size_t i(0); i < marks.size(); ++i) {
                        combination += genome[marks[i]-1];
                    }
                    all_combinations.push_back(combination);
                    combination.clear();
                }
            }
            confstr.close();
            return all_combinations;
        } 
        else {
            throw("Could not open FASTA file " + file_to_read);
        }
    } 
    catch(std::ifstream::failure &e) {
        throw("Error with FASTA file " + file_to_read + ": " + e.what());
    }
}

std::map<std::string, double> FastaReader::freq_per_allele() 
{
    std::map<std::string, double> alleles_freq;
    std::vector<std::string> all_combinations_ = readFile();
    N = all_combinations_.size();
    double init_freq(1.0/N);

    for (size_t i(0); i < all_combinations_.size(); ++i) {
        alleles_freq[all_combinations_[i]] += init_freq;
    }
    return alleles_freq;
}