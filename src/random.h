#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>

/*! \class RandomNumbers
  This is a random number class based on standard c++-11 generators.

  This header declares the global variable \ref main.cpp "_RNG", a pointer to the unique instance of this class.
 */

class RandomNumbers
{
public:
  /*! @name Initializing
  The generator \ref rng is a Mersenne twister *mt19937* engine. 

  A \ref seed *s>0* can be provided, by default it is seeded with a *random_device*.
 */
  ///@{
  RandomNumbers(unsigned long int s = 0);
  ///@}

  /*! @name Distributions
  These functions either a single number generated according to the corresponding distribution.
  The parameters are the standard parameters of these distributions.
 */
  ///@{
  /*! Uniform distribution function
   \param lower (double): lower bond of the distribution interval, 0 by default
   \param upper (double): upped bond of the distribution interval, 1 by degault
   \return a random number contained in the interval defined by the parameters
   */
  double uniform_double(double lower = 0, double upper = 1);
  
 /*! Binomial distribution function
  \param n (int): sample size
  \param p (double): propability of succes
  \return a random number generated by the binomial distribution
  */
  size_t binomial(int n, double p);

  /*! Multinomial distribution law
  \param N (int): sample size
  \param n_frequence (std::vector<double>&): frequencies that need to be modified by the function
  \param fitness_ fitness coefficients used for natural selection (>0 is favourable, 
  between -1 and 0 is unfavourable and 0 or -1 is lethal)
  \return new frequencies, randomly generated using a combination of binomial
  */
  std::vector<double> multinomial(const int& N, std::vector<double> n_frequence, const std::vector<double>& fitness_);
    ///@}
    
  /*! @name Auxiliary function
  Generates a random letter correspponding to a nucleotide (A, C, T or G) 
  using the uniform distribution for the interval of 1 to 4
  \return std::string a different letter for each number
 */
  ///@{
  std::string randomLetter();
  ///@}

private:
  std::mt19937 rng;
  long int seed;
};

extern RandomNumbers *_RNG;

#endif
