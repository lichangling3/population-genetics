#ifndef SIMULATION_H
#define SIMULATION_H
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

class Simulation
{
public:
  /*! 
  Constructor used when no Fasta file is provided.
  It will generate as many Population as repetitions, each initialised with the given parameters
 \param pop_size_ (int): used to initialise a Population
 \param sim_duration_ (int): used to initialise \ref sim_duration attribute
 \param nb_alleles_ (int): used to initialise a Population
 \param nb_freq_ (std::vector <double>): used to initialise a Population
 \param repetitions_ (int): used to initialise \ref repetition attribute
 \param new_fit(std::vector <double>): used to initialise a Population
 \see Population::setSize(int)
 \see Population::setAlleles (std::vector<double>, int)
 \see Population::setFitness (std::vector<double>)
 */
  Simulation(int pop_size_, int sim_duration_, int nb_alleles_, std::vector<double> nb_freq_, int repetitions_, std::vector<double> new_fit);

  /*! 
  Constructor used when a Fasta fille is provided.
   It will generate as many Population as repetitions, each initialised with the given parameters.
  \param file_name_ (std::string): name of the Fasta file containing the alleles
  \param nb_marks: (std::vector<int>): used to initialise Population
  \param sim_duration_ (int): used to initialise \ref sim_duration attribute
  \param repetitions_ (int): used to initialise \ref repetition attribute
  \param new_fit(std::vector <double>): used to initialise a Population
  \param new_mu(std::vector <double>): used to initialise \ref marks_mu attribute
  \param sites (std::vector<int>): sites that should be mutated
  \param delta (double): chances of being mutated to one nucleotide or another are proportional to delta (Kimura model)
  \param isMutation_ (bool): used to initialise the isMutation attribute
  \param alleles (Alleles): used to initialise a Population
  \see Population::setSize (int)
  \see Population::setWithFile (Alleles)
 */

  Simulation(std::string file_name_, std::vector<int> nb_marks_, int sim_duration_, int repetitions_, std::vector<double> new_fit, std::vector<double> mu, std::vector<int> sites, double delta_, bool isMutation_, Alleles alleles, double mu_default_);

  /// Method that runs the simulation itself
  /*! This method calls Display to show the evolution of a number of alleles in a Population.
   If mutations are wanted, they will happen here.
   \see Population::step()
   \see Population::mutation
   \see Display
  */
  void run();

private:
  /// \see Population
  std::vector<Population> populations;
  ///length of the simulation
  int sim_duration;
  ///marks of the alleles to read if Fasta file provided
  std::vector<int> nb_marks;
  ///chances of being mutated to one nucleotide or another are proportional to delta (Kimura model)
  double delta;

  /// a vector containg pairs of marks indexes (int) and the corresponding mutation rates
  std::vector<std::pair<int, double>> marks_mu;

  ///number of generations in a population
  int repetitions;
  ///file to which the output will be written
  std::ofstream my_flow;

  ///boolean to see if mutations occur
  bool isMutation;
};

#endif
