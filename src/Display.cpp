#include "Display.h"
#include "Simulation.h"
#include <iomanip>

Display::Display() {}

void Display::displayGen(const Population& pop, std::ofstream &write)
{
	if (write.is_open())
	{
		write << "\t";
		for (auto x : pop.getpopAlleles())
		{
			write << x.second << "|";
		}
	}
	else
	{
		std::cerr << "error while opening text file" << std::endl;
	}
}

void Display::displayAll(const Population& pop, std::ofstream &write)
{
	if (write.is_open())
	{
		write << "\t";
		for (auto x : pop.getpopAlleles())
		{
			write << x.first << "|";
		}
	}
	else
	{
		std::cerr << "error while opening text file" << std::endl;
	}
}
