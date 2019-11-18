#include "FastaReader.h"
#include <iostream>

FastaReader::FastaReader() {}

std::vector<std::string> FastaReader::readFile(std::vector<size_t> marks, const std::string file_to_read) {
    std::vector<std::string> all_combinations;
    try {
        std::ifstream confstr(file_to_read);
        if (confstr.is_open()) {
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
                    genome.clear();
                } else if(!id.empty()) {
                    genome += line;
                    // This removes all spaces from the string
                    genome.erase(std::remove_if(genome.begin(), genome.end(), isspace), genome.end());
                    // This puts all the letters in upper case
                    std::transform(genome.begin(), genome.end(), genome.begin(), ::toupper);
                    for (size_t i(0); i < marks.size(); ++i) {
                        if (genome[marks[i]-1] == 'N') {
                            combination += randomNucleotide();
                        } else {
                            combination += genome[marks[i]-1];
                        }
                    }
                    all_combinations.push_back(combination);
                    combination.clear();
                } else {
                    throw std::runtime_error("It seems that you did not provide a FASTA file.");
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
//Corriger pk il calcule 2x ?
std::string FastaReader::randomNucleotide() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, 4);
    int number = distribution(gen);
    std::string nucleotide = "";
    switch (number) {
        case 1 :
            nucleotide = 'A';
            break;
        case 2 :
            nucleotide = 'C';
            break;
        case 3 :
            nucleotide = 'T';
            break;
        case 4 :
            nucleotide = 'G';
            break;
    }
    return nucleotide;
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


