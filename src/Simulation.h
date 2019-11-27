#include <vector>
#include <Population.h>
#include <fstream>
#include <FastaReader.h>
#include <Display.h>
#include <iomanip>

/*! \class Simulation
 This is a class that simulates the process of genetic drift.
 The class builds Population and reproduces the effect time would have on the frequency of alleles present in that population.
 The class has 2 constructors, according to whether a Fasta file is provided or not. 
 */

class Simulation {

public :
/*! 
  Constructor used when no Fasta file is provided.
  It will generate as many Population as repetitions, each initialised with the given parameters.
 \param pop_size_ (size_t): used to initialise a Population.
 \param sim_duration_ (size_t): used to initialise \ref sim_duration attribute.
 \param nb_alleles_ (size_t): used to initialise a Population.
 \param nb_freq_ (std::vector <double>): used to initialise a Population.
 \param repetitions_ (size_t): used to initialise \ref repetition attribute.
 \see Population::setSize(size_t)
 \see Population::setAlleles (std::vector<double>,  size:t);
 */
Simulation(size_t pop_size_, size_t sim_duration_, size_t nb_alleles_, std::vector<double> nb_freq_, size_t repetitions_);

/*! 
  Constructor used when a Fasta fille is provided.
   It will generate as many Population as repetitions, each initialised with the given parameters.
  \param file_name_ (std::string): name of the Fasta file containing the alleles.
  \param nb_marks: (std::vector<size_t>): used to initialise Population.
  \param sim_duration_ (size_t): used to initialise \ref sim_duration attribute.
  \param repetitions_ (size_t): used to initialise \ref repetition attribute.
  \see Population::setSize (size_t)
  \see Population::setWithFile (Alleles);
 */
Simulation(std::string file_name_, std::vector<size_t> nb_marks_, size_t sim_duration_, size_t repetitions_);

/// Method that runs the simulation itself
/*! This method calls Display to show the evolution of a number of \typedef Alleles in a Population.
 \see Population::step()
 \see Display
  */
void run();

private :
/// \see Population 
std::vector<Population> populations; 

size_t sim_duration;
size_t pop_size;
std::vector<size_t> nb_marks;

///number of generations in a population
size_t repetitions;
std::ofstream my_flow;
static size_t gen_counter;

};
