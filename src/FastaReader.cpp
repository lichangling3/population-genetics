#include "FastaReader.h"
#include <iostream>

FastaReader::FastaReader() {}

std::vector<std::string> FastaReader::readFile(std::vector<size_t> marks, const std::string file_to_read) {
    std::vector<std::string> all_combinations;
    try {
        std::ifstream confstr(file_to_read);
        if (confstr.is_open()) {
            RandomNumbers random;
            std::sort(marks.begin(), marks.end());
            std::string line, id, genome, combination;
            while(std::getline(confstr, line)) {
                if(line[0] == '>') { 
                    if(!id.empty()) { 
                        id.clear();
                    }
                    if(!line.empty()) {
                        id = line.substr(1);
                    }
                    if (!genome.empty()) {
                        for (size_t i(0); i < marks.size(); ++i) {
                            if (marks[i]-1 > genome.size()) {
                                throw std::runtime_error("The marks provided are not suitable for the genomes");
                            } else if (marks[i] == 0) {
                                throw std::runtime_error("The value of marks start at 1");
                            } else if (!(genome[marks[i]-1] == 'A' || genome[marks[i]-1] == 'C' || genome[marks[i]-1] == 'T' || genome[marks[i]-1] == 'G')) {
                                combination += random.randomLetter();
                            } else {
                                combination += genome[marks[i]-1];
                            }
                        }
                        all_combinations.push_back(combination);
                        combination.clear();
                        genome.clear();
                    }
                } else if(!id.empty()) {
                    // This removes all spaces from the string
                    line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
                    // This puts all the letters in upper case
                    std::transform(line.begin(), line.end(), line.begin(), ::toupper);
                    genome += line;
                } else {
                    throw std::runtime_error("It seems that you did not provide a FASTA file.");
                }
            }
            if (!genome.empty()) {
                for (size_t i(0); i < marks.size(); ++i) {
                    if (marks[i]-1 > genome.size()) {
                        throw std::runtime_error("The marks provided are not suitable for the genomes");
                    } else if (marks[i] == 0) {
                        throw std::runtime_error("The value of marks start at 1");
                    } else if (genome[marks[i]-1] == 'N') {
                        combination += random.randomLetter();
                    } else {
                        combination += genome[marks[i]-1];
                    }
                }
                all_combinations.push_back(combination);
                combination.clear();
                genome.clear();
                id.clear();
                line.clear();
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

std::map<std::string, double> FastaReader::retrieveData(std::vector<size_t> marks, const std::string file_to_read) {
    std::map<std::string, double> alleles_freq;
    std::vector<std::string> all_combinations_ = readFile(marks, file_to_read);
    size_t N = all_combinations_.size();
    double init_freq(1.0/N);

    for (size_t i(0); i < all_combinations_.size(); ++i) {
        alleles_freq[all_combinations_[i]] += init_freq;
    }
    return alleles_freq;
}

size_t FastaReader::size(std::vector<size_t> marks, const std::string file_to_read) {
    return readFile(marks, file_to_read).size();
}


