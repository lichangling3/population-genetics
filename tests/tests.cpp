#include <gtest/gtest.h>
#include "FastaReader.h"
#include "random.h"

TEST(readFileTest, freq_per_allele) 
{
	std::map<std::string, double> freq_received = FastaReader::freq_per_allele({1,3,6}, "../tests/test.fasta");
	std::map<std::string, double> freq_test;
	freq_test["ACG"] = 0.25;
	freq_test["AGG"] = 0.25;
	freq_test["TGC"] = 0.5;
	int N_test = 4;

	std::map<std::string, double>::const_iterator i, j;
	for(i = freq_received.begin(), j = freq_test.begin(); j != freq_test.end(); ++i, ++j) {
		EXPECT_EQ(i->first, j->first);
		EXPECT_DOUBLE_EQ(i->second, j->second);
	}
	EXPECT_EQ(N_test, FastaReader::size({1,3,6}, "../tests/test.fasta"));
}

TEST(Random, multinomial){
	RandomNumbers rng;
	//write rest of the test
}
