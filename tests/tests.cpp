#include <gtest/gtest.h>
#include "FastaReader.h"
#include "random.h"
#include "Simulation.h"
#include "Display.h"
#include "fstream"

RandomNumbers* _RNG;
Simulation* _SIM;


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
	//write rest of the test
}

TEST(Display, displayGen) {
	std::ofstream my_flow = std::ofstream("../tests/test_for_display");
	std::map<std::string,double> alleles;
	for(double i(1); i < 5; ++i) {
		std::string s = std::to_string(i);
		alleles[s] = (i/10);
	}
	Population pop(alleles);
	Display::displayGen(pop, my_flow);
	std::vector<std::string> split;
	std::string line, f;
	try {
        std::ifstream confstr("../tests/test_for_display");
        if (confstr.is_open()) {
            while(std::getline(confstr, line)) {
				std::stringstream ss(line);
				while(std::getline(ss, f, '|')) {
					split.push_back(f);
				}
			}
        } 
        else {
            throw("Could not open FASTA file ../tests/test_for_display");
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


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
