#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <random>

#ifndef FASTAREADER_H
#define FASTAREADER_H


/*! \class FastaReader
 * This is a class that reads information provided in a Fasta file.
 */

class FastaReader
{
	public:
	/*! @name Initializing
  The constructor does not need any arguments and is empty as FastaReader does not have any attribuites.
 */
///@{
	FastaReader();
	///@}

/*! @name Retrieving functions
  These functions allow \ref to retrieve data form the input Fasta file.
  * \ref retrieveData takes a vector of marks that divides the alleles of the file and the name of the file. It returns a map of alleles and the corresponding initial frequency.
  * \ref size takes a vector of marks that divides the alleles of the file and the name of the file. It returns the population size.
 */
 ///@{
	static std::map<std::string, double> retrieveData(std::vector<size_t> marks, const std::string file_to_read); //sets N
	static size_t size(std::vector<size_t> marks, const std::string file_to_read); 
	///@}
	
/*! @name Private functions
 * These functions are used by \ref retrieveData and \ref size. They should not be used outside of the class.
 * */
	private:
/*! 
 * Reads the provided Fasta file and returns a string containing all alleles in the provided file. In case the file contains an undefined nucleotide (N), it is replaced by a random nucleotide using \ref randomNucleotide.
 * \param marks (std::vector<size_t>): marks demarcating the alleles.
 * \param file_to_read (std::string): name of the Fasta file containing the data.
 * \return a string containing all alleles in the provided file, all in capital letters and without spaces.
  */
	static std::vector<std::string> readFile(std::vector<size_t> marks, const std::string file_to_read); 
	
	/*! 
	 * Generates a random number between 1 and 4 using the normal distribution and returns a different nucleotide for each number
	 */
	 ///@{
	static std::string randomNucleotide();
	///@}
};

#endif //FASTAREADER_H
