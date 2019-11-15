#include "random.h"
#include <random>

RandomNumbers::RandomNumbers (unsigned long int s)
: seed (s)
{
	if (seed == 0){
		std::random_device rd;
		seed = rd();
	}
	rng = std::mt19937(seed);
}

void RandomNumbers::uniform_double(std::vector<double>& numbers, double lower, double upper){
	std::uniform_real_distribution<> dist (lower, upper); 
	for (auto I = numbers.begin(); I != numbers.end(); I++){ 
		*I = dist(rng);
	}
}

double RandomNumbers::uniform_double(double lower, double upper){
	std::uniform_real_distribution<> dist (lower, upper); 
	return dist(rng);
}

void RandomNumbers::normal(std::vector<double>& numbers, double mean, double sd){
	std::normal_distribution<> dist (mean, sd);
	for (auto I = numbers.begin(); I != numbers.end(); I++){ 
		*I = dist(rng);
	}
}

double RandomNumbers::normal(double mean, double sd){
	std::normal_distribution<> dist (mean, sd);
	return dist (rng);
}

void RandomNumbers::poisson(std::vector<int>& numbers, double mean){
	std::poisson_distribution <> dist (mean);
	for (auto I = numbers.begin(); I != numbers.end(); I++){ 
		*I = dist(rng);
	}
}

int RandomNumbers::poisson(double mean){
	std::poisson_distribution <> dist (mean);
	return dist(rng);
}


double RandomNumbers::binomial (int n, double p) {
	std::binomial_distribution <> dist (n,p);
	return dist (rng);
}

std::vector<double> RandomNumbers::multinomial (std::vector<double> n_frequence)
{
	int N(0);
	for (auto n : n_frequence)
	{
		N += n;
	}
	
	std::vector<double> k_frequence;
	int new_k(0);
	double old_n (N);
	int new_n (N);
	int sum_k(0);
	double p;
	
	for (size_t i(0); i < n_frequence.size(); ++i)
	{
		
		p = (n_frequence[i]/old_n);
		new_k = binomial(new_n , p);
		old_n -= n_frequence[i];
		new_n -= new_k; 
		
		k_frequence.push_back(new_k);
		sum_k += new_k;
	}
	
	k_frequence.push_back(N - sum_k);
	
	for (auto freq : k_frequence) { //mutinomial return now the frequency of the alleles in the population 
		freq /= N;
	}


return k_frequence;	
}
