#ifndef DISPLAY_H
#define DISPLAY_H

#include "Population.h"
#include <iostream>
#include <fstream>

/// \class Display
 /*! This class displays all alleles in a population with the corresponding frequencies
 */

class Display{
public:
/*! @name Initializing
  The constructor does not need any arguments and is empty as Display does not have any attributes.
 */
	Display();
	
///@name Displaying functions
/*! 
  Displays the frequencies of all the alleles of a Population. 
  \param pop (Population): population where the alleles and their frequencies can be found.
  \param write (std::ofstream&): ofstream where the frequencies will be written.
 */
	static void displayGen(Population pop, std::ofstream& write);
	
/*! 
  Displays the code of all the alleles of Population. 
  \param pop (Population): population where the alleles and their frequencies can be found.
  \param write (std::ofstream&): ofstream where the frequencies will be written.
 */
	static void displayAll(Population pop, std::ofstream& write);
	
};	
	
#endif
