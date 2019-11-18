#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>

/*! \class RandomNumbers
  This is a random number class based on standard c++-11 generators.

  This headers declares the global variable \ref main.cpp "_RNG", a pointer to the unique instance of this class.
 */

class RandomNumbers {

public:
/*! @name Initializing
  The generator \ref rng is a Mersenne twister *mt19937* engine. 

  A seed *s>0* can be provided, by default it is seeded with a *random_device*.
 */
///@{
    RandomNumbers(unsigned long int s=0);
///@}

/*! @name Distributions
  These functions either return a single number 
  or fill a given vector with random numbers distributed 
  according the specified distributions. 

  The additional parameters are the standard parameters of these distributions.
 */
///@{
    void uniform_double(std::vector<double>&, double lower=0, double upper=1);
    double uniform_double(double lower=0, double upper=1);
    void normal(std::vector<double>&, double mean=0, double sd=1);
    double normal(double mean=0, double sd=1);
    void poisson(std::vector<int>&, double mean=1);
    int poisson(double mean=1);
    double binomial (int n, double p);
    std::vector<double> multinomial (std::vector<double>);
    std::vector<double> multinomialDistribution(double N,std::vector<double> freq);
    std::vector<double> multinomials(std::vector<double> f, unsigned int N);
    std::vector<double> multibinomial (int N, std::vector<double> f);
    std::vector<double> multinomiale(int N, std :: vector<double> effectif_allele);
    void multinomial(std::vector<double> old_repartition, std::vector<double> new_repartition);
    double sum(std::vector<double> vec);
    std::vector<double> multinomialeTest(int N, std :: vector<double> effectif_allele);

///@}
/*! @name Auxiliary function
  This takes a vector of indices and re-orders it randomly.
 */
///@{
    void shuffle(std::vector<size_t> &_v) {std::shuffle(_v.begin(), _v.end(), rng);}
///@}
     
private:
    std::mt19937 rng;
    long int seed;

};

extern RandomNumbers* _RNG;

#endif
