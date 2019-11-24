#include <gtest/gtest.h>
#include "FastaReader.h"
#include "random.h"
#include "Simulation.h"
#include <fstream>
#include "Display.h"
RandomNumbers* _RNG;


TEST(readFileTest, retrieveData) 
{
	std::map<std::string, double> freq_received = FastaReader::retrieveData({1,3,6}, "../tests/test_for_retrieveData.fasta");
	std::map<std::string, double> freq_test;
	freq_test["ACG"] = 0.25;
	freq_test["AGG"] = 0.25;
	freq_test["TGC"] = 0.5;
	size_t N_test = 4;

	std::map<std::string, double>::const_iterator i, j;
	for(i = freq_received.begin(), j = freq_test.begin(); j != freq_test.end(); ++i, ++j) {
		EXPECT_EQ(i->first, j->first);
		EXPECT_DOUBLE_EQ(i->second, j->second);
	}
	EXPECT_EQ(N_test, FastaReader::size({1,3,6}, "../tests/test_for_retrieveData.fasta"));
}

TEST(Random, multinomial){
	
	RandomNumbers rng;
	int N (10);
	std::vector<double> n_frequence {0.1, 0.4, 0.3, 0.2};
	std::vector<double> new_frequence (rng.multinomial(N, n_frequence)) ;
	double total_freq (0.0);
	for (auto freq: new_frequence) {
		total_freq += freq;
	}
	
	EXPECT_NEAR(1, total_freq, 0.0001);
}


TEST (Random, multinomial_average_freq){
	RandomNumbers RN;
	
	size_t replications (30); // enter number of replications, knowing that a higher number means higher precision 
	
	std::vector<double> init_freq{0.2, 0.4};	
	std::vector<double> alleles;
	
	for (size_t i(0); i<replications; ++i) {
		std::vector<double> new_freq = RN.multinomial(20, init_freq);
		for (size_t i(0); i<new_freq.size(); ++i){
			alleles[i] += new_freq[i];
			}
	}
	
	for (size_t j(0); j<= alleles.size(); ++j){
		j= j / init_freq.size();
		
		EXPECT_NEAR(alleles[j], init_freq[j], 0.05);		//enter precision according to nbr of replications
	}
	
	
}


TEST(Random, fixation){
	
	RandomNumbers rng;
	int N (10);
	std::vector<double> freq {0.1, 0.4, 0.0, 0.3,  0.2};
	std::vector<double> new_frequence (rng.multinomial(N, freq)) ;

	EXPECT_EQ(0, new_frequence[2]);
}

TEST(Display, displayGen) {
	Population pop;
	std::ofstream my_flow = std::ofstream("../tests/test_for_display.txt");
	Alleles alleles;
	for(double i(1); i < 5; ++i) {
		std::string s = std::to_string(i);
		alleles[s] = (i/10);
	}
	pop.setPopAlleles(alleles);
	Display::displayGen(pop, my_flow);
	std::vector<std::string> split;
	std::string line, f;
	my_flow.close();
	try {
        std::ifstream confstr("../tests/test_for_display.txt");
        if (confstr.is_open()) {
            while(std::getline(confstr, line)) {
				std::stringstream ss(line);
				while(std::getline(ss, f, '|')) {
					split.push_back(f);
				}
			}
        } 
        else {
            throw("Could not open FASTA file ../tests/test_for_display.txt");
        }
    } 
    catch(std::ifstream::failure &e) {
        throw("Error with FASTA file ../tests/test_for_display:" and e.what());
    }
	EXPECT_EQ(split[0],"0.1");
	EXPECT_EQ(split[1],"0.2");
	EXPECT_EQ(split[2],"0.3");
	EXPECT_EQ(split[3],"0.4");
}

TEST(GlobalTest, SmallTest)
{
    //without fasta file
    Simulation sim(4, 0, 3, {0.4, 0.5, 0.1}, 3);
    sim.run();

    try {
        std::ifstream confstr("display.txt");
        if (confstr.is_open()) {
            std::string line;
            while(std::getline(confstr, line)) {
				if (line[0] == '0') {
					EXPECT_EQ(line, "0              0.4|0.5|0.1|0.4|0.5|0.1|0.4|0.5|0.1|");
				} else {
					EXPECT_EQ(line, "                 1|2|3|    1|2|3|    1|2|3|");
				}
            }
        } else {
            throw std::runtime_error("Could not open display.txt");
        }
    } catch(std::ifstream::failure &e) {
        throw("Error with display.txt file " and e.what());
    }
    
    //with fasta file
    Simulation sim_file("../tests/test_for_retrieveData.fasta", {1, 3, 6}, 0, 3);
    sim_file.run();

    try {
        std::ifstream confstr("display.txt");
        if (confstr.is_open()) {
            std::string line;
            while(std::getline(confstr, line)) {
				if (line[0] == '0') {
					EXPECT_EQ(line, "0              0.25|0.25|0.5|0.25|0.25|0.5|0.25|0.25|0.5|");
				} else {
					EXPECT_EQ(line, "               ACG|AGG|TGC|  ACG|AGG|TGC|  ACG|AGG|TGC|");
				}
            }
        } else {
            throw std::runtime_error("Could not open display.txt");
        }
    } catch(std::ifstream::failure &e) {
        throw("Error with display.txt file " and e.what());
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


