#ifndef FASTAREADER_H
#define FASTAREADER_H
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <random>
#include "random.h"

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
    \param marks(vector<int>): divide the alleles of the file 
    \param file_to_read (string): the name of the file
    \return map of alleles and the corresponding initial frequency.
    \see readFile
  */
  static std::map<std::string, double> retrieveData(std::vector<int> marks, const std::string file_to_read);

  /*! Retrieves the size of the Population in the provided Fasta file.
  \param marks (vector<int>): divide the alleles of the file 
  \param file_to_read (string) name of the file 
  \return population size.
*/
  static size_t size(std::vector<int> marks, const std::string file_to_read);

  /*! @name Private function
  This function is used by retrieveData and size. It should not be used outside of the class.
 */
private:
  /*! 
  Reads the provided Fasta file and returns a string containing all alleles in the provided file. 
  In case the file contains a letter that is not a DNA base (not A, C, G or T), it is replaced 
  by a random nucleotide using RandomNumbers::randomLetter().
  \param marks (vector<int>):  marks demarcating the alleles.
  \param file_to_read (string): name of the Fasta file containing the data.
  \return a string containing all alleles in the provided file, all in capital letters and without spaces.
  \see RandomNumbers::randomLetter()
  */
  static std::vector<std::string> readFile(std::vector<int> marks, const std::string file_to_read);
};

#endif //FASTAREADER_H