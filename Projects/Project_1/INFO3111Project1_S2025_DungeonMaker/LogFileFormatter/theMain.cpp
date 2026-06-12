#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: Log file is the 1st param (or drop log file onto exe)" << std::endl;
		return -1;
	}

	std::string inputFile(argv[1]);
	std::ifstream inFile(inputFile);
	if (!inFile.is_open())
	{
		std::cout << "Error: can't open file." << std::endl;
		return -1;
	}

	std::vector<std::string> vecInFile;
	std::string lineBuffer;

	while(getline(inFile, lineBuffer))
	{
		vecInFile.push_back(lineBuffer);
		lineBuffer.clear();	
	}

	const unsigned int MAZE_1_OFFSET = 5;
	const unsigned int MAZE_2_OFFSET = 26;
	const unsigned int MAZE_3_OFFSET = 47;
	const unsigned int MAZE_4_OFFSET = 68;
	const unsigned int MAZE_5_OFFSET = 89;
	const unsigned int MAZE_NUMBER_OF_LINES = 18;

	const unsigned int PROPS_OFFSET = 112;
	const unsigned int NUMBER_OF_PROPS = 10;


	for (unsigned int index = 0; index <= MAZE_NUMBER_OF_LINES; index++)
	{
		vecInFile[index + MAZE_1_OFFSET] += "    ";
		vecInFile[index + MAZE_1_OFFSET] += vecInFile[index + MAZE_2_OFFSET];
		vecInFile[index + MAZE_1_OFFSET] += "    ";
		vecInFile[index + MAZE_1_OFFSET] += vecInFile[index + MAZE_3_OFFSET];

		vecInFile[index + MAZE_2_OFFSET] = vecInFile[index + MAZE_4_OFFSET];
		vecInFile[index + MAZE_2_OFFSET] += "    ";
		vecInFile[index + MAZE_2_OFFSET] += vecInFile[index + MAZE_5_OFFSET];
		vecInFile[index + MAZE_2_OFFSET] += "    ";
	}

	for (unsigned int index = 0; index <= NUMBER_OF_PROPS; index++)
	{
		vecInFile[index + MAZE_2_OFFSET] += vecInFile[index + PROPS_OFFSET];
	}

	std::cout << vecInFile[1] << std::endl;
	std::cout << std::endl;
	for (unsigned int index = MAZE_1_OFFSET; index <= (MAZE_2_OFFSET + MAZE_NUMBER_OF_LINES); index++)
	{
		std::cout << vecInFile[index] << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;

	system("pause");

//	for ( unsigned int index = 112; index != 122; index++ )
//	{
//		std::cout << vecInFile[index] << std::endl;
//	}	
	
	// 1st maze is line  5 to 23 : 18
	// 2nd maze is line 26 to 44 : 18
	// 3rd maze is line 47 to 65 : 18
	// 4th maze is line 68 to 86 : 18
	// 5th maze is line 89 to 97
	// Props:
	//	102 to 111



	return 0;
}
