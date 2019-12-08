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
  It will generate as many Population as repetitions, each initialized with the given parameters
 \param pop_size_ (size_t): used to initialize a Population
 \param sim_duration_ (size_t): used to initialize \ref sim_duration attribute
 \param nb_alleles_ (size_t): used to initialize a Population
 \param nb_freq_ (std::vector <double>): used to initialize a Population
 \param repetitions_ (size_t): used to initialize \ref repetition attribute
 \param new_fit(std::vector <double>): used to initialize a Population
 \see Population::setSize(size_t)
 \see Population::setAlleles (std::vector<double>,  size:t)
 \see Population::setFitness (std::vector<double>)
 */
  Simulation(size_t pop_size_ = 100, size_t sim_duration_ = 10, size_t nb_alleles_ = 2, std::vector<double> nb_freq_ = std::vector<double>{0.8, 0.2}, size_t repetitions_ = 2, std::vector<double> new_fit = std::vector<double>{0.0, 0.0});

  /*! 
  Constructor used when a Fasta fille is provided.
   It will generate as many Population as repetitions, each initialized with the given parameters.
  \param file_name_ (std::string): name of the Fasta file containing the alleles
  \param nb_marks: (std::vector<size_t>): used to initialize Population
  \param sim_duration_ (size_t): used to initialize \ref sim_duration attribute
  \param repetitions_ (size_t): used to initialize \ref repetition attribute
  \param new_fit(std::vector <double>): used to initialize a Population
  \param new_mu(std::vector <double>): used to initialize \ref marks_mu attribute
  \see Population::setSize (size_t)
  \see Population::setWithFile (Alleles)
 */
  Simulation(std::string file_name_, std::vector<size_t> nb_marks_, size_t sim_duration_, size_t repetitions_, std::vector<double> new_fit, std::vector<double> mu, std::vector<size_t> sites, double delta_);

  /// Method that runs the simulation itself
  /*! This method calls Display to show the evolution of a number of \typedef Alleles in a Population.
 \see Population::step()
 \see Display
  */
  void run();

private:
  /// \see Population
  std::vector<Population> populations;

  size_t sim_duration;
  std::vector<size_t> nb_marks;
  double delta;

  /// a vector containg pairs of marks indexes (size_t) and the corresponding mutation rates
  std::vector<std::pair<size_t, double>> marks_mu;

  ///number of generations in a population
  size_t repetitions;
  std::ofstream my_flow;
  static size_t gen_counter;
};

#endif
