#include "Display.h"
#include "Simulation.h"
#include <iomanip>

Display::Display() {}

void Display::displayGen(const Population& pop, std::ofstream &write) {
	write << "\t";
	for (auto x : pop.getpopAlleles()) {
		write << x.second << "|";
	}
}

void Display::displayAll(const Population& pop, std::ofstream &write) {
	write << "\t";
	for (auto x : pop.getpopAlleles()) {
		write << x.first << "|";
	}
}
