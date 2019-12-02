#include "Display.h"
#include "Simulation.h"
#include <iomanip>

Display::Display() {}

void Display::displayGen(Population pop, std::ofstream &write)
{
	if (write.is_open())
	{
		for (auto x : pop.getpopAlleles())
		{
			write << x.second << "|";
		}
		write << std::right << std::setw(3);
	}
	else
	{
		std::cerr << "error while opening text file" << std::endl;
	}
}

void Display::displayAll(Population pop, std::ofstream &write)
{
	if (write.is_open())
	{
		for (auto x : pop.getpopAlleles())
		{
			write << x.first << "|";
		}
		write << std::right << std::setw(5);
	}
	else
	{
		std::cerr << "error while opening text file" << std::endl;
	}
}