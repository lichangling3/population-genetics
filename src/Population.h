#ifndef POPULATION_H
#define POPULATION_H
#include <iostream>
#include <vector>
#include <map>
#include "random.h"

typedef std::map<std::string, double> Alleles;

/*! \class Population
 * A population is a \typedef Alleles "set" of alleles and has a \ref size size that is equal to the total number of alleles.
 * \typedef Alleles are a typedef of a map. This map contains the allele's sequence as a string, and its frequency as a double.
 * 
 * A population is created in two different manners, according to whether a Fasta file is provided or not. 
 * */

class Population {
public :

/*! @name Initializing
  The constructor does not need any arguments and is empty as attributes will be assigned in other functions.
  * \ref setAlleles sets the frequencies of the alleles contained in \ref PopAlleles to those of the \param std::vector<double>.
  * \ref setWithFile sets \ref PopAlleles to \param Alleles.
  * \ref setPopAlleles sets \ref PopAlleles to \param std::map<std::string, double>.
*/
///@{
	Population(){};
	void setAlleles(size_t nb_alleles, std::vector<double> nb_freq_);
	void setWithFile(Alleles popAlleles_);
	void setSize(size_t size_);
	void setPopAlleles(std::map<std::string, double> map);
	///@}
	
	/*! @name Getters
	 */
 ///@{
	size_t getSize()const{return size;};
	size_t getAllelesSize() const {return popAlleles.size();};
	Alleles getpopAlleles()const{return popAlleles;};
	///@}	
		
/*! Assignes a random new frequency to each allele of the population using the \ref multinomial method of \ref RandomGenerator.
 */	
  ///@{		
	void step();
	///@}
private :
	Alleles popAlleles;	
	size_t size;
	
};

#endif
