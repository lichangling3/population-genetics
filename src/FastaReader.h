#include <string>
#include <vector>

#ifndef FASTAREADER_H
#define FASTAREADER_H

class FastaReader
{
	public:
	FastaReader();	//constructor without arguments
	FastaReader(std::vector<int> marks_, std::string file_to_read_);	//constructor with arguments used essentialy for tests
	
	private:
	std::vector<int> marks;		//I expect marks given as a vector
	std::string file_to_read;	//I expect file_to_read given as a string
};


#endif //FASTAREADER_H
