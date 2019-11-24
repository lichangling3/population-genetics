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

class Population {
public :

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
	
	void setSize(size_t size_);
	///@}
	
	/*! @name Getters
	 */
 ///@{
	size_t getSize()const{return size;};
	size_t getAllelesSize() const {return popAlleles.size();};
	Alleles getpopAlleles()const{return popAlleles;};
	///@}	
		
/// @name Evolution function
/*! Assignes a random new frequency to each allele of the population using the multinomial method of RandomNumbers.
 \see RandomNumbers::multinomial (int, vector<double>&).
 */	
  ///@{		
	void step();
	///@}
private :
///typedef of a map with a string as key (sequence) and a double as value (frequency)
	Alleles popAlleles;	
	size_t size;
	
};

#endif
