#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <vector>
#include <map>
#include "random.h"
typedef std::map<std::string, double> Alleles;

/*! \class Population
 A population is a "set" of alleles of \ref size equal to the total number of alleles.

 A population is created in two different manners, according to whether a Fasta file is provided or not. 
 */

class Population
{
public:
	/*! @name Initializing
  The constructor sets the parameter values to the corresponding attributes.
  \param popAlleles_
  \param size_
  \param fitness_
*/
	///@{
	Population(const Alleles &popAlleles_, int size_, const std::vector<double> &fitness_);

	/*!  Sets the frequencies of the alleles contained in \ref PopAlleles to those of nb_freq_
	 \param nb_freq_ (std::vector<double>): frequencies of alleles
	 \param nb_alleles (int): number of alleles
*/
	static Alleles setAlleles(int nb_alleles, const std::vector<double>& nb_freq_);

	/// @name Getters
	///@{
	int getSize() const { return size; };
	int getAllelesSize() const { return popAlleles.size(); };
	Alleles getpopAlleles() const { return popAlleles; };
	std::vector<double> getFitness() const { return fitness; };
	///@}

	/// @name Evolution function
	/*! Assignes a random new frequency to each allele of the population using the multinomial method of RandomNumbers.
   \see RandomNumbers::multinomial (int, vector<double>&).
 */
	///@{
	void step();
	///@}

	///@name Mutation functions
	///@{
	/*! Mutates all alleles that need to be mutated according to user's input.
	\param marks (std::vector<std::pair<snt, double>>): mark and the corresponding mutation rate
	\param delta (double): used to calculate mutation probabilities (see mainpage)
*/
	void mutation(const std::vector<std::pair<int, double>> &marks, double delta);

	/*! Changes a base to another, each of the other bases having a probability depending on delta.
	 \param base the base that needs to be mutated
	 \param delta probability of each base to be chosen is (1-delta)/2
	 \return a new base, different from the parameter
	 */
	char modelMut(char base, double delta);
	///@}

private:
	///typedef of a map with a string as key (sequence) and a double as value (frequency)
	Alleles popAlleles;
	///size of the population
	int size;

	///the fitness coefficient of each allele. It is assumed that these coefficients correspond to the alleles of popAlleles (same order)
	std::vector<double> fitness;
};

#endif
