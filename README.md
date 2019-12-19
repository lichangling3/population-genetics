# Team_1
SV-Projects in C++ 2019, team no 1  
Project n°1 : Genetic of Populations (Wright-Fisher) ReadMe
[Team n°1]
## I. What the program does
The main goal of this program is to simulate the temporal evolution of a population
represented by N individuals with a genome. In order to do that, we will follow the model of
Wright-Fisher. There is a fixed number A of alleles in the population and each individual has
a type of allele. At each generation, we will sort a new population with a size N among the
individuals of the last generation according to the multinomial distribution. The alleles of
this population can mutate during the simulation (with mutation coefficients) or can be more
or less selected (with fitness coefficients).
## II. The software you need to install
* CMake (compilation)
* Doxygen (documentation)
* Gtest (tests)
* TCLAP (command on terminal)
## III. How to execute the program
First, after cloning the project, you need to create a folder “build” and compile the
program in this folder with the commands (in the terminal from the folder "build") : 
“cmake ..” and then “make” before starting the program, which is named "Populationgenetic". 
You can perform the test with the command "./PopulationTest" from the folder "build" you created.
If you want to look at the documentation Doxygen you have to create a folder named "doc" then 
execute the command "make doc" in the terminal.
You have two possible ways to perform the simulation : with our without a fasta file.
If you do not want to work with a fasta file you need to enter all the factors the simulation
needs. :
* The time of simulation or the number of generations with -T (default value : 10)
* The number of repetitions of the simulation with -R (default value : 2)
* The size of the population with -N (default value : 100)
* The number of alleles of the population with -A (default value : 2)
* The frequencies of the alleles with -f (no default value)
* The fitness coefficient of the alleles with -S (default value : 0 for each allele)  

If you want to use a fasta file, you can decide to add mutations or not. So without mutations, you need to precise : 
* The time of simulation or the number of generations with -T (default value : 10)
* The number of repetitions of the simulation with -R (default value : 2)
* The name of the fasta file with -F (default value : ../fasta/test_for_retrieveData.fasta)
* The marks of the nucleotides of the alleles with -m (no default value)
* The fitness coefficient of the alleles with -S (default value : 0 for each allele)

However, only the marks of the nucleotides are obligatory because the other parameters are set by default.
3) If you want to use a fasta file and to add mutations, you have two possible ways to perform the simulation : 
- You can give the same mutation rate for all the sites indicated by the marks, in this case you only need to precise : 
* The parameters for a simulation with fasta file without mutations
* The mutation rate by default with -D (default value : 0, should be between 0 and 1)
- You can specify the mutation rate of the sites one by one, in this case you also need to precise :
* The parameters for a simulation with fasta file without mutations
* The mutation rates with with -M for each site specified (should be between 0 and 1)
* The marks of the sites mutated with -s  
* The mutation rate by default with -D (default value : 0, should be between 0 and 1) for all the sites you didn’t specify

Examples of command :
- Without a fasta file and with selection :
*./PopulationGenetic -R 4 -T 100 -A 4 -f 0.1 -f 0.1 -f 0.3 -f 0.5 -N 100 -S -1 -S 0 -S -1 -S 0.4  
Here we set a population of size 100,  with 4 replications, during 100 generations, with 4 alleles in which the frequencies are 0.1, 0.1, 0.3 and 0.5. We also add coefficients of selection for each allele (-1, 0, -1 and 0.4).

- With a fasta file and mutations and without selection :
*./PopulationGenetic -F ../test.fa -m 2  -m 4 -m 8 -T 100 -R 2 -M 0.01 -m 2 -d 0.5 -D 0.0001
Here we work with the fasta file test.fa, and we will perform the simulation during 100 generations with 2 replications. We also add one coefficient of mutations (0.01) on the site “2”, a delta coefficient for the mutations 0.5 and a coefficient of mutations of 0.001 for the other sites of mutations.
