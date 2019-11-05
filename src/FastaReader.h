#include <string>
#include <vector>

#ifndef FASTAREADER_H
#define FASTAREADER_H

class FastaReader
{
	public:
	//two constructors are here provided. You can choose the one you prefer depending on the context and preference
	FastaReader();	//constructor without arguments
	FastaReader(std::vector<int> marks, string file_to_read);	//constructor with arguments
	
	private:
	std::vector<int> marks;		//I expect marks given as a vector
	std::string file_to_read;	//I expect file_to_read given as a string
};


#endif //FASTAREADER_H
