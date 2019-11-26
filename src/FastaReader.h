#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <random>
#include "random.h"

#ifndef FASTAREADER_H
#define FASTAREADER_H


/*! \class FastaReader
 This is a class that reads information provided in a Fasta file.
 */

class FastaReader
{
	public:
/*! @name Initializing
  The constructor does not need any arguments and is empty as FastaReader does not have any attributes.
 */
	FastaReader();

/*! @name Retrieving functions
  These functions allow to retrieve data from the input Fasta file.
 */
  
/*! Retrieves the alleles' sequences and their frequencies from the provided Fasta file.
    \param marks(vector<size_t>): divide the alleles of the file 
    \param file_to_read (string): the name of the file
    \return map of alleles and the corresponding initial frequency.
    \see readFile
  */
 	static std::map<std::string, double> retrieveData(std::vector<size_t> marks, const std::string file_to_read);
 	 
/*! Retrives the size of the Population in the provided Fasta file.
  \param marks (vector<size_t>): divide the alleles of the file 
  \param file_to_read name (string): of the file 
  \return population size.
*/
	static size_t size(std::vector<size_t> marks, const std::string file_to_read); 
	
	
/*! @name Private functions
  These functions are used by retrieveData and size. They should not be used outside of the class.
 */
	private:
/*! 
  Reads the provided Fasta file and returns a string containing all alleles in the provided file. In case the file contains an undefined nucleotide (N), it is replaced by a random nucleotide using \ref randomNucleotide.
  \param marks (vector<size_t>):  marks demarcating the alleles.
  \param file_to_read (string): name of the Fasta file containing the data.
  \return a string containing all alleles in the provided file, all in capital letters and without spaces.
  */
	static std::vector<std::string> readFile(std::vector<size_t> marks, const std::string file_to_read); 

};

#endif //FASTAREADER_H
