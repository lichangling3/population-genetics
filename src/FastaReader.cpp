#include "FastaReader.h"

FastaReader::FastaReader()
:marks(_MARKS_), file_to_read(_FILE_TO_READ_);		//I expect _MARKS_ and _FILE_TO_READ_ as global elements
{}

FastaReader::FastaReader(std::vector<int> marks_, std::string file_to_read_)
:marks(marks_), file_to_read(file_to_read_)
{}
