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
 * */

class Population
{
public:
	/*! @name Initializing
  The constructor does not need any arguments and is empty as attributes will be assigned in other functions.
*/
	///@{
	Population(){};

	/*!  Sets the frequencies of the alleles contained in \ref PopAlleles to those of nb_freq_
	 \param nb_freq_  frequencies of alleles
	 \param nb_alleles: number of alleles
*/
	void setAlleles(size_t nb_alleles, std::vector<double> nb_freq_);

	/*! Sets \ref PopAlleles to the parameter 
	\param popAlleles_ (Alleles): map with a string as key (sequence) and a double as value (frequency)
*/
	void setWithFile(Alleles popAlleles_);

	/*! Sets \ref PopAlleles to the parameter 
	\param map (map<string, double>): should contain allele sequences and frequencies
*/
	void setPopAlleles(std::map<std::string, double> map);
	void setFitness(std::vector<double> new_fit);
	void setSize(size_t size_);
	///@}

	/*! @name Getters
	 */
	///@{
	size_t getSize() const { return size; };
	size_t getAllelesSize() const { return popAlleles.size(); };
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
  This function uses one of the model mut function depending on the provided parameters.
  \param marks mark and the corresponding mutation rate
  \param delta optionnal parameter, depending on the chosen mutation model
*/
	void mutation(std::vector<std::pair<size_t, double>> marks, double delta);

	/*! Changes a base to another, each of the other bases having a probability of 1/3 to be chosen.
	 \param base the base that needs to be mutated
	 \return a new base, different from the parameter
	 */
	char modelMut(char base);

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
	size_t size;

	///the fitness coefficient of each allele. We assume that these coefficients correspond to the alleles of popAlleles (same order)
	std::vector<double> fitness;
};

#endif