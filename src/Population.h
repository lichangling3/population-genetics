#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <vector>
#include <map>
#include "random.h"

typedef std::map<std::string, double> Alleles;

/*! \class Population
 * A population is a "set" of alleles and has a \ref size that is equal to the total number of alleles.
 * 
 * A population is created in two different manners, according to whether a Fasta file is provided or not. 
 * */

class Population {
public :

/*! @name Initializing
  The constructor does not need any arguments and is empty as attributes will be assigned in other functions.
*/
///@{
	Population(){};
	
	
	/*!  sets the frequencies of the alleles contained in \ref PopAlleles to those of nb_freq_
	 * \param nb_freq_
	 * \param nb_alleles: number of alleles
	*/
	void setAlleles(size_t nb_alleles, std::vector<double> nb_freq_);
	
	///sets \ref PopAlleles to the parameter \param popAlleles_
	 void setWithFile(Alleles popAlleles_);
	 
	///  sets \ref PopAlleles to the parameter \param map
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
		
/*! Assignes a random new frequency to each allele of the population using the multinomial method of RandomNumbers.
 * \see RandomNumbers::multinomial (int N, std::vector<double>& n_frequence).
 */	
  ///@{		
	void step();
	///@}
private :
	Alleles popAlleles;	///typedef of a map with a string as key and a double as value
	size_t size;
	
};

#endif
