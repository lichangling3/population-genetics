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

std::vector<double> RandomNumbers::multinomialDistribution(double N,std::vector<double> freq) {
	double summK(0),summn(0),normalisation(1/N);
	std::vector<double> tabQuantite;
	for(auto x:freq){
		tabQuantite.push_back(x*N);
	}
	for(auto& x : tabQuantite){
		double temp(x);
		if((N-summn)== 0){
			x=0;
		}else{
			x = (binomial(N-summK,x/(N-summn)));
		}
		summK += x;
		summn += temp;
	}
	for(size_t i(0); i<tabQuantite.size(); ++i){
		freq[i] = tabQuantite[i]*normalisation;
	}
	return freq;
}


std::vector<double> RandomNumbers::multinomials(std::vector<double> f, unsigned int N){
	
	std::vector<double> resultat;
	
	double n = N;
	double new_freq(0.0);
	double sumFreq(0.0);
	
	for (size_t i(0); i<f.size()-1; ++i){
		if(n==0){ 
			new_freq = 0.0;
		}else{
			std::binomial_distribution<> d(n, f[i]*N/n);
			double d_ = d(rng);
			n = n-d_;
			new_freq = d_/N;		
		}
		resultat.push_back(new_freq);
		sumFreq += new_freq;
	}
	
	f.back() = 1 - sumFreq;				
	/*assert((sumFreq <= 1.0)); //controler que sumFreq ne peut pas etre plus grand que 1*/
	
	resultat.push_back(f.back());
	/*assert((resultat.size()==f.size())); //grandeur du tableau*/
	
	return resultat; 
}

std::vector<double> RandomNumbers::multibinomial (int N, std::vector<double> f)
{
	std::vector<double> new_frequences;
	for(size_t i(0); i< f.size()-1; ++i) {
		if(f[i] == 0 ) {
			new_frequences.push_back(0.0);
		} else {
			double mean(N);
			double var(f[i]);
			double tmps(N);
		
			for(size_t j(0); j<i; ++j) {
				mean -= new_frequences[j];
				tmps -=f[j];
			}
		
			var = var/tmps;
			std::binomial_distribution<int> distribution(mean*100, var); //pour avoir le chiffres réels
		
			new_frequences.push_back(distribution(rng)*0.01); //on transforme en pourcentage
		}
	}
	if(f[f.size() -1] != 0.0) { 
		double f_last(1);
		for(auto s : new_frequences) {
			f_last -= s;
		}
		if (f_last < 0.0) {f_last = 0.0;}
		new_frequences.push_back(f_last);
	}
	else {
		new_frequences.push_back(0.0);
	}
	
	return new_frequences;
}

std :: vector<double> RandomNumbers::multinomiale(int N, std :: vector<double> effectif_allele){

    std :: vector<double> nouveaux_effectifs(effectif_allele.size(), 0);

    int k(0);
    int taille(N);

    for (size_t i(0); i < effectif_allele.size() - 1; ++i) {
        std :: binomial_distribution<int> distribution(N + k, (double) effectif_allele[i]/taille);
        nouveaux_effectifs[i] = distribution(rng);

        k -= nouveaux_effectifs[i];
        taille -= effectif_allele[i];
    }

    nouveaux_effectifs[nouveaux_effectifs.size() - 1] = N + k;


    return nouveaux_effectifs;
}


void RandomNumbers::multinomial(std::vector<double> old_repartition, std::vector<double> new_repartition)
{
	size_t N = old_repartition.size();
	
	size_t sum_new(0);
	size_t sum_old(0);
	for (size_t i(0); i< old_repartition.size(); ++i) {
		if (N != sum_old) {
			int newcount = binomial(N - sum_new, old_repartition[i]/(N - sum_old));
			new_repartition[i] = newcount;
			sum_new += newcount;
		} else {
			new_repartition[i] = 0;
		}
		
		sum_old += old_repartition[i];

	}
}

double RandomNumbers:: sum(std::vector<double> vec)
{
	double sum(0);
	for(auto x:vec)
	{
		sum += x;
	}
	return sum;
}


std :: vector<double> RandomNumbers::multinomialeTest(int N, std :: vector<double> effectif_allele) {
	int nb_tirages(0);
	double proba(0);
	double sum1(0);
	double sum2(0);
	N *= 100;
	for(size_t i(0); i < effectif_allele.size(); ++i)
	{
		effectif_allele[i] *= 100;
	}
	std::vector<double> final_allele(effectif_allele.size(), 0);
	for(size_t i(0); i < effectif_allele.size(); ++i)
	{
		if((N - sum(effectif_allele)) > 0)
		{
			nb_tirages = (N - sum(final_allele));
			proba = effectif_allele[i] / (N - sum(effectif_allele));
			final_allele[i] = binomial(nb_tirages, proba);
		}
		else {
			final_allele[i] = (N - sum(final_allele));
		}
	}
	return final_allele;
}
