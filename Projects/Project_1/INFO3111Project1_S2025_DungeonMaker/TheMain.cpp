#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <sstream>
#include "cMazeMaker.h"
#include "cMersenneTwister.h"
#include <algorithm>

using namespace std;

std::ofstream theFile;

cMersenneTwister* pRandom = NULL;

void consumeNnumbers(unsigned int count);

void print(std::string theText, bool newLine = false)
{
	theFile << theText;
	cout << theText;

	if (newLine)
	{
		theFile << endl;
		cout << endl;
	}
	return;
}

std::string intToString(unsigned int theInt)
{
	std::stringstream ssTheInt;
	ssTheInt << theInt; 
	return ssTheInt.str();
}

int randBetweenRange(int min, int max)
{
	//int output = min + (rand() % static_cast<int>(max - min + 1));
	int output = min + (pRandom->getNextRand() % static_cast<int>(max - min + 1));
	return output;
}

std::string genDungeon(unsigned int SN)
{
//	srand(SN);
	
	cMazeMaker maze(pRandom);
	maze.GenerateMaze(30, 15);

	int count = 0;
	while (count < 100)
	{
		cRoom room(pRandom);	
		room.genRoom(2, 2, 8, 5);

		maze.AddRoom(room);

		count++;
	};


	return maze.getPrintedMaze();
}

//std::string genCoridor(unsigned int SN)
//{
//	// Reset seed to student number
//	srand(SN);
//
//	print("Your corridor has the following curved wall sections:", true);
//	print("(so SM_Env_Wall_Curved_xx model)", true);
//	print("", true);
//
//	for (unsigned int count = 0; count < 4; count++)
//	{
//		print("Section ");
//		print(intToString(count));
//		print(" : ");
//		print("left: ");
//		print(intToString(randBetweenRange(1, 5)));
//		print("  right: ");
//		print(intToString(randBetweenRange(1, 5)), true);
//	}
//	print("", true);
//
//	return "";
//}
 
std::string genProps_Dungeons(unsigned int SN)
{
	// Reset seed to student number
	//srand(SN);
	consumeNnumbers(SN);

	print("Add the following props to your dungeon:", true);
	print("", true);

	std::vector<std::string> vecItems;

	for (unsigned int count = 0; count < 20; count++)
	{
		int prop = randBetweenRange(0, 28);
	
		switch (prop)
		{
		case 0: case 1: case 2:
			vecItems.push_back("Bag");
			break;
		case 3:	case 4: case 5: case 6:
			vecItems.push_back("Bottle"); 
			break;
		case 7:		
			vecItems.push_back("Bottle_Candle"); 
			break;
		case 8:		
			vecItems.push_back("Candelabra"); 
			break;
		case 9:		
			vecItems.push_back("Candle"); 
			break;
		case 10: case 11: case 12:
			vecItems.push_back("Chalice, cup, tankard, mug, etc.");
			break;
		case 13:	
			vecItems.push_back("Compass"); 
			break;
		case 14:	
			vecItems.push_back("Anvil"); 
			break;
		case 15: case 16: case 17:
			vecItems.push_back("Barrel");
			break;
		case 18: case 19:
			vecItems.push_back("Lantern");
			break;
		case 20:	
			vecItems.push_back("Skull"); 
			break;
		case 21: case 22: case 23: case 24:
			vecItems.push_back("Dead Body");
			break;
		case 25:	
			vecItems.push_back("Skeleton Cage "); 
			break;
		case 26: case 27:
			vecItems.push_back("Skeleton");
			break;
		default: 
			vecItems.push_back("Bottle");
		}
		

	}//for (unsigned int count = 0; count < 10; count++)

	// Print out the items
	print("How many? : The item", true);
	print("", true);

	while (!vecItems.empty())
	{
		std::string theItem = vecItems[0];

		int itemCount = std::count_if(vecItems.begin(), vecItems.end(),
			[theItem](std::string testItem)
			{
				return (testItem == theItem);
			}
		);

		print(intToString(itemCount), false);
		print(" : ", false);
		print(theItem, true);
		
		// "Remove item" i.e. replace the value with ""
		std::vector<std::string>::iterator itNewEnd = std::remove(vecItems.begin(), vecItems.end(), theItem);
		// (remove returns new end iterator, but won't remove items from vector)
		vecItems.erase(itNewEnd, vecItems.end());

	}//while (!vecItems.empty())

	print("", true);
		
		

	return "";
}

 
std::string genProps_SpaceEnvironment(unsigned int SN)
{
	// Reset seed to student number
//	srand(SN);
	consumeNnumbers(SN);

	print("Add the following props to your hangar:", true);
	print("", true);

	for (unsigned int count = 0; count < 10; count++)
	{
		int prop = randBetweenRange(0, 32);
	
		switch (prop)
		{
		case 0:	print("SM_Prop_Scales_01", true); break;
		case 1:	print("SM_Prop_Server_01", true); break;
		case 2:	print("SM_Prop_Server_02", true); break;
		case 3:	print("SM_Prop_Server_03", true); break;
		case 4:	print("SM_Prop_Cart_01", true); break;
		case 5:	print("SM_Prop_Plants_05", true); break;
		case 6:	print("SM_Prop_Plants_04", true); break;
		case 7:	print("SM_Prop_Plants_03", true); break;
		case 8:	print("SM_Prop_Plants_02", true); break;
		case 9:	print("SM_Prop_Plants_01", true); break;
		case 10:	print("SM_Prop_Lockers_01", true); break;
		case 11:	print("SM_Prop_Lockers_02", true); break;
		case 12:	print("SM_Prop_Lockers_03", true); break;
		case 13:	print("SM_Prop_Lockers_04", true); break;
		case 14:	print("SM_Prop_Lockers_05", true); break;
		case 15:	print("SM_Prop_ServerRack_01", true); break;
		case 16:	print("SM_Prop_Sign_01", true); break;
		case 17:	print("SM_Prop_StepLadder_01", true); break;
		case 18:	print("SM_Prop_3DPrinter_01", true); break;
		case 19:	print("SM_Prop_Table_01", true); break;
		case 20:	print("SM_Prop_Treadmill_01", true); break;
		case 21:	print("SM_Prop_Trolley_01", true); break;
		case 22:	print("SM_Prop_Rocket_01", true); break;
		case 23:	print("SM_Prop_Phone_01", true); break;
		case 24:	print("SM_Prop_Monitor_04", true); break;
		case 25:	print("SM_Prop_Monitor_03", true); break;
		case 26:	print("SM_Prop_Monitor_02", true); break;
		case 27:	print("SM_Prop_Monitor_01", true); break;
		case 28:	print("SM_Prop_Lockers_01", true); break;
		case 29:	print("SM_Prop_SwivelChair_01", true); break;
		case 30:	print("SM_Prop_SwivelChair_02", true); break;
		case 31:	print("SM_Prop_SwivelChair_03", true); break;
		default:	print("SM_Prop_SwivelChair_04", true); break;
		}
		

	}//for (unsigned int count = 0; count < 10; count++)


	print("", true);
		
		

	return "";
}



int main()
{

//	srand( (unsigned int)time(NULL));


	cout << "Welcome to the INFO-3111 project 1 Summer 2025 maze generator thingy." << endl;
	cout << endl;
	cout << "What's your student number, student? ";

	unsigned int SN = 0;
	cin >> SN;

	cout << endl;
	cout << "You entered: " << SN << endl;
	cout << endl;
	cout << "If that's wrong, please exit the program and start again." << endl;
	cout << "(Note: the file name will change as it's based on the time and SN)" << std::endl;
	cout << endl;

	unsigned int timeRightNow = (unsigned int)time(NULL);
//	srand((unsigned int)time(NULL));

	pRandom = new cMersenneTwister(timeRightNow);

	std::stringstream ssLogFileName;
//	ssLogFileName << "INFO-3111_log_" << rand() << ".log";
	ssLogFileName << "INFO-3111_log_" << pRandom->getNextRand() << ".log";

	theFile.open(ssLogFileName.str().c_str(), std::fstream::out);
	if (!theFile.is_open())
	{
		cout << "Error: Couldn't open file." << endl;
		cout << "Be sure to run this in a folder with write permissions" << endl;
		cout << "Exiting" << endl;
		return -1;
	}
	
	print("Log file opened", true);

	print("Student number: ", false);
	print(intToString(SN), true);

	print("Time: ", false);
	print(intToString(timeRightNow), true);
	print("", true);

	bool bKeepGoing = true;

	// Reset seed to student number
//	srand(SN);
	consumeNnumbers(SN);

	print("Please choose one of these five dungeons:", true);
	print("", true);

	std::string d[5];
	for ( int index = 0; index != 5; index++ )
	{
		d[index] = genDungeon(SN+index-2);
		print(d[index],true);
		print("",true);
	}

	genProps_Dungeons(SN);

	theFile.close();

	delete pRandom;

	system("pause");

	return 0;
}

void consumeNnumbers(unsigned int count)
{
	if (pRandom)
	{
		for (unsigned int n = 0; n < count; n++)
		{
			pRandom->getNextRand();
		}
	}
	return;
}

