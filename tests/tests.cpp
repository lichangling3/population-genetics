#include <gtest/gtest.h>
#include "FastaReader.h"
#include "random.h"
#include "Simulation.h"
#include <fstream>
#include "Display.h"
RandomNumbers *_RNG;

TEST(readFileTest, retrieveData)
{
	std::map<std::string, double> freq_received = FastaReader::retrieveData({1, 3, 9}, "../tests/test_for_retrieveData.fasta");
	std::map<std::string, double> freq_test;
	freq_test["ACA"] = 0.25;
	freq_test["AGA"] = 0.25;
	freq_test["TGT"] = 0.5;
	size_t N_test = 4;

	std::map<std::string, double>::const_iterator i, j;
	for (i = freq_received.begin(), j = freq_test.begin(); j != freq_test.end(); ++i, ++j)
	{
		EXPECT_EQ(i->first, j->first);
		EXPECT_DOUBLE_EQ(i->second, j->second);
	}
	EXPECT_EQ(N_test, FastaReader::size({1, 3, 9}, "../tests/test_for_retrieveData.fasta"));
}

TEST(Random, multinomial)
{
	int N(10);
	std::vector<double> n_frequence{0.1, 0.4, 0.3, 0.2};
	std::vector<double> new_frequence(_RNG->multinomial(N, n_frequence, {0, 0, 0, 0}));
	double total_freq(0.0);
	for (auto freq : new_frequence)
	{
		total_freq += freq;
	}
	EXPECT_NEAR(1, total_freq, 0.0001);
}

TEST(Random, fixation)
{
	int N(10);
	std::vector<double> freq{0.1, 0.4, 0.0, 0.3, 0.2};
	std::vector<double> new_frequence(_RNG->multinomial(N, freq, {0, 0, 0, 0, 0}));
	EXPECT_EQ(0, new_frequence[2]);
}

TEST(Display, displayGen)
{
	Population pop;
	std::ofstream my_flow = std::ofstream("../tests/test_for_display.txt");
	Alleles alleles;
	for (double i(1); i < 5; ++i)
	{
		std::string s = std::to_string(i);
		alleles[s] = (i / 10);
	}
	pop.setPopAlleles(alleles);
	Display::displayGen(pop, my_flow);
	std::vector<std::string> split;
	std::string line, f;
	my_flow.close();
	try
	{
		std::ifstream confstr("../tests/test_for_display.txt");
		if (confstr.is_open())
		{
			while (std::getline(confstr, line))
			{
				std::stringstream ss(line);
				while (std::getline(ss, f, '|'))
				{
					split.push_back(f);
				}
			}
		}
		else
		{
			throw("Could not open FASTA file ../tests/test_for_display.txt");
		}
	}
	catch (std::ifstream::failure &e)
	{
		throw("Error with FASTA file ../tests/test_for_display:" and e.what());
	}
	EXPECT_EQ(split[0], "0.1");
	EXPECT_EQ(split[1], "0.2");
	EXPECT_EQ(split[2], "0.3");
	EXPECT_EQ(split[3], "0.4");
}

TEST(GlobalTest, SmallTest)
{
	//without fasta file
	Simulation sim(4, 0, 3, {0.4, 0.5, 0.1}, 3, {0, 0, 0});
	sim.run();
	try
	{
		std::ifstream confstr("display.txt");
		if (confstr.is_open())
		{
			std::string line;
			while (std::getline(confstr, line))
			{
				if (line[0] == '0')
				{
					EXPECT_EQ(line, "0              0.4|0.5|0.1|0.4|0.5|0.1|0.4|0.5|0.1|");
				}
				else
				{
					EXPECT_EQ(line, "                 1|2|3|    1|2|3|    1|2|3|");
				}
			}
		}
		else
		{
			throw std::runtime_error("Could not open display.txt");
		}
	}
	catch (std::ifstream::failure &e)
	{
		throw("Error with display.txt file " and e.what());
	}
	//with fasta file
	Simulation sim_file("../tests/test_for_retrieveData.fasta", {1, 3, 6}, 0, 3, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0);
	sim_file.run();
	try
	{
		std::ifstream confstr("display.txt");
		if (confstr.is_open())
		{
			std::string line;
			while (std::getline(confstr, line))
			{
				if (line[0] == '0')
				{
					EXPECT_EQ(line, "0              0.25|0.25|0.5|0.25|0.25|0.5|0.25|0.25|0.5|");
				}
				else
				{
					EXPECT_EQ(line, "               ACG|AGG|TGC|  ACG|AGG|TGC|  ACG|AGG|TGC|");
				}
			}
		}
		else
		{
			throw std::runtime_error("Could not open display.txt");
		}
	}
	catch (std::ifstream::failure &e)
	{
		throw("Error with display.txt file " and e.what());
	}
}

TEST(Random, multinomial_average_freq)
{
	size_t replications(10000); // enter number of replications, knowing that a higher number means higher precision
	int size_pop(200000);
	std::vector<double> init_freq{0.7, 0.1, 0.2};
	std::vector<double> alleles(init_freq.size(), 0);
	std::vector<double> new_freq;
	for (size_t i(0); i < replications; ++i)
	{
		new_freq = _RNG->multinomial(size_pop, init_freq, {0, 0, 0});
		for (size_t n(0); n < alleles.size(); ++n)
		{
			alleles[n] += new_freq[n];
		}
	}
	for (size_t j(0); j < alleles.size(); ++j)
	{
		alleles[j] /= replications;
		EXPECT_NEAR(alleles[j], init_freq[j], 0.05); //enter precision according to nbr of replications
	}
}

TEST(mutation, new_frequencies)
{
	Population Pop;
	Alleles alleles;
	std::vector<std::pair<size_t, double>> marks{std::make_pair(1, 0.01), std::make_pair(2, 0.01), std::make_pair(3, 0.01)};
	alleles["ACG"] = 0.2;
	alleles["TCC"] = 0.3;
	alleles["GAT"] = 0.5;

	int init_size(alleles.size());

	Pop.setPopAlleles(alleles);
	Pop.setSize(800);

	Pop.mutation(marks);

	Alleles new_pop(Pop.getpopAlleles());
	int new_size(Pop.getAllelesSize());
	double sum(0.0);

	EXPECT_TRUE(new_size - init_size >= 0);

	if (new_size - init_size == 0)
	{
		std::cout << "TEST IRREVELENT, there wasn't any new allele(s) created this time..." << std::endl;
	}
	if (new_size - init_size > 0)
	{
		std::map<std::string, double>::const_iterator it, IT;

		for (it = new_pop.begin(), IT = alleles.begin(); it != new_pop.end(); ++it, ++IT)
		{
			sum += it->second;

			if (alleles.count(it->first))
			{
				int number(it->second * Pop.getSize() + 0.1);
				EXPECT_NEAR(number, it->second * Pop.getSize(), 0.000001);
			}
			else
			{
				int number(it->second * Pop.getSize() + 0.1);
				EXPECT_NEAR(number, it->second * Pop.getSize(), 0.000001);
			}
		}
		EXPECT_NEAR(sum, 1.0, 0.00001);
	}
}

TEST(Mutation, modelMut)
{
	Population Pop;
	Alleles alleles;
	std::vector<std::pair<size_t, double>> marks;
	for (size_t i(1); i < 21; ++i)
	{
		marks.push_back(std::make_pair(i, 0.05));
	}
	alleles["ACCTTACGAATACGGATCAC"] = 0.2;
	alleles["TCAATTCAGCCGAATCAGCA"] = 0.3;
	alleles["TACGTACTACTATAAGACAC"] = 0.3;
	alleles["CACATACGACTGAAGAGCCT"] = 0.1;
	alleles["AATACTTCAGGCCCCGAAGC"] = 0.1;

	Pop.setPopAlleles(alleles);
	Pop.setSize(1000);
	std::vector<double> fit(5, 0);
	Pop.setFitness(fit);

	std::map<char, size_t> init_map;
	init_map['A'] = 0;
	init_map['C'] = 0;
	init_map['T'] = 0;
	init_map['G'] = 0;
	std::map<std::string, double>::const_iterator it;
	for (it = alleles.begin(); it != alleles.end(); ++it)
	{
		for (size_t i(0); i < 20; ++i)
		{
			init_map[it->first[i]] += 1;
		}
	}
	size_t sum_AC = init_map['A'] + init_map['C'];
	size_t sum_TG = init_map['T'] + init_map['G'];
	EXPECT_GT(sum_AC, sum_TG);

	size_t timer = 120;
	for (size_t i(0); i < timer; ++i)
	{
		Pop.step();
		Pop.mutation(marks, 0.99);
	}

	Alleles mutated_alleles(Pop.getpopAlleles());
	init_map.clear();
	sum_AC = 0;
	sum_TG = 0;
	size_t counter(0);
	std::map<std::string, double>::reverse_iterator rit;
	for (rit = mutated_alleles.rbegin(); rit != mutated_alleles.rend(); ++rit)
	{
		if (counter > 4)
			break;
		counter += 1;
		for (size_t i(0); i < 20; ++i)
		{
			init_map[rit->first[i]] += 1;
		}
	}
	sum_AC = init_map['A'] + init_map['C'];
	sum_TG = init_map['T'] + init_map['G'];
	EXPECT_LT(alleles.size(), mutated_alleles.size());
	EXPECT_LT(sum_AC, sum_TG);

	init_map.clear();
	std::vector<char> mut_vec;
	for (size_t j(0); j < 1000; ++j)
	{
		mut_vec.push_back(Pop.modelMut('C', 0.6));
		init_map[mut_vec[j]] += 1;
	}
	EXPECT_GT(init_map['T'], init_map['A']);
	EXPECT_GT(init_map['T'], init_map['C']);
	EXPECT_GT(init_map['T'], init_map['G']);
	EXPECT_EQ(init_map['C'], 0.0);
}

int main(int argc, char **argv)
{
	_RNG = new RandomNumbers();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}