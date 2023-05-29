// main.c

#include "Header.h"

// Function prototypes
int NumberOfNeighbors(Flat imatFlBuilding[][FLATS_PER_FLOOR], int inFloorIndex,
	int inFlatIndex);
int AreNeighborsEnemies(imatFlBuilding, inFloorIndex, inFlatIndex, inAnimalType);

//---------------------------------------------------------------------------------------
//                                     A Flat For Rent
//                                    -----------------
//
// General : The program receives list of animals to accommodate in building.
//
// Input   : Input file sends the animals to accommodate by order in format of animal
//			 type and maximum number of neighbors requests when moving to the building.
// 
// Process : The program uses data structures to organise building's current state and
//			 uses that state to determine the location of next animal from file.
//
// Output  : The program prints final state of building to output file.
// 
//---------------------------------------------------------------------------------------
// Programmer : Lahav Gafni
// Student No : 325761880
// Date       : 26.11.2022
//---------------------------------------------------------------------------------------
void main()
{
	// Variable definition
	int nFloorIndex;
	int nFlatIndex;
	int nAnimalType;
	char* pcOutputFileName;
	AnimalPtr pAnmlNewAnimal;
	Flat matFlBuilding[FLOORS][FLATS_PER_FLOOR];
	FILE* pfsInputFile;
	FILE* pfsOutputFile;

	// Code section

	// Initializing building.
	for (nFloorIndex = 0; nFloorIndex < FLOORS; nFloorIndex++)
	{
		for (nFlatIndex = 0; nFlatIndex < FLATS_PER_FLOOR; nFlatIndex++)
		{
			 matFlBuilding[nFloorIndex][nFlatIndex].anmltAnimalType = NONE;
		}
	}
	pfsInputFile = fopen(FILE_NAME, "rb");
	fscanf(pfsInputFile, "%s", pcOutputFileName);
	pfsOutputFile = fopen(pcOutputFileName, "at");
	// Reading data from file.
	while(!feof(pfsInputFile))
	{
		// Allocating memory for new animal.
		if (!(pAnmlNewAnimal = (AnimalPtr)malloc(sizeof(Animal))))
		{
			printf("Error 1: Insufficient memory");
			exit(1);
		}
		fread(&nAnimalType, sizeof(AnimalType), 1, pfsInputFile);
		fread(&pAnmlNewAnimal->nMaxNeighbors, sizeof(int), 1, pfsInputFile);
		// Accommodating new animal.
		for (nFloorIndex = 0; nFloorIndex < FLOORS; nFloorIndex++)
		{
			for (nFlatIndex = 0; nFlatIndex < FLATS_PER_FLOOR; nFlatIndex++)
			{
				// Scanning for ideal flat.
				if (!matFlBuilding[nFloorIndex][nFlatIndex].anmltAnimalType ||
					NumberOfNeighbors(matFlBuilding, nFloorIndex, nFlatIndex) <=
					pAnmlNewAnimal->nMaxNeighbors &&
					!AreNeighborsEnemies(matFlBuilding, nFloorIndex, nFlatIndex,
										 nAnimalType))
				{
					matFlBuilding[nFloorIndex][nFlatIndex].anmlAnimal = pAnmlNewAnimal;
					matFlBuilding[nFloorIndex][nFlatIndex].anmltAnimalType = nAnimalType;
				}
			}
		}
		// Checking if flat was found for animal.
		if (nFloorIndex == FLOORS && nFlatIndex == FLATS_PER_FLOOR &&
			!matFlBuilding[FLOORS - 1][FLATS_PER_FLOOR - 1].anmltAnimalType)
		{
			printf("Error 2: No flat found for animal");
			exit(2);
		}
	}
	// Printing building to output file.
	fprintf(pfsOutputFile, "{");
	for (nFloorIndex = 0; nFloorIndex < FLOORS; nFloorIndex++)
	{
		if (!nFloorIndex)
		{
			fprintf(pfsOutputFile, " {");
		}
		else
		{
			fprintf(pfsOutputFile, "  {");
		}
		for (nFlatIndex = 0; nFlatIndex < FLATS_PER_FLOOR - 1; nFlatIndex++)
		{
			fprintf(pfsOutputFile, " %d,",
					matFlBuilding[nFlatIndex][nFloorIndex].anmltAnimalType);
		}
		if (nFloorIndex < FLOORS - 1)
		{
			fprintf(pfsOutputFile, " %d }\n",
				matFlBuilding[nFlatIndex][nFloorIndex].anmltAnimalType);
		}
		else
		{
			fprintf(pfsOutputFile, " %d }",
				matFlBuilding[nFlatIndex][nFloorIndex].anmltAnimalType);
		}
	}
	fprintf(pfsOutputFile, " }");
}

//---------------------------------------------------------------------------------------
//                                   Number of Neighbors
//                                  ---------------------
//
// General      : The function calculates amount of neighbors for given index.
//
// Parameters   : 
//      imatFlBuilding - The main data structure (In)
//		inFloorIndex   - Floor index (In)
//		inFlatIndex	   - Flat index (In)
//
// Return Value : Final number of neighbors
// 
//---------------------------------------------------------------------------------------
int NumberOfNeighbors(Flat imatFlBuilding[][FLATS_PER_FLOOR], int inFloorIndex,
					  int inFlatIndex)
{
	// Variable definition
	int nFloorIndex;
	int nFlatIndex;
	int nNewFloorIndex;
	int nNewFlatIndex;
	int nNeighbors;

	// Code section

	// Iterating through every neighboring flat for given indexes.
	for (nNeighbors = 0, nFloorIndex = -1, nNewFloorIndex = inFloorIndex + nFloorIndex;
		nFloorIndex <= 1 && nNewFloorIndex < FLOORS;
		nFloorIndex++, nNewFloorIndex = inFloorIndex + nFloorIndex)
	{
		// Iterated index must be in range.
		if (nNewFloorIndex < 0);
		else for (nFlatIndex = -1, nNewFlatIndex = inFlatIndex + nFlatIndex;
			nFlatIndex <= 1 && nNewFlatIndex < FLATS_PER_FLOOR;
			nFlatIndex++, nNewFlatIndex = inFlatIndex + nFlatIndex)
		{
			// Iterated index must be in range.
			if (nNewFlatIndex < 0);
			// Adding detected neighbor.
			else if (imatFlBuilding[nNewFloorIndex][nNewFlatIndex].anmltAnimalType)
			{
				nNeighbors++;
			}
		}
		return (nNeighbors);
	}
}

//---------------------------------------------------------------------------------------
//                                  Are Neighbors Enemies
//                                 -----------------------
//
// General      : The program determines if animal in given index is compatible with
//				  animals in neighboring flats.
//
// Parameters   : 
//      imatFlBuilding   - The main data structure (In)
//		inFloorIndex     - Floor index (In)
//		inFlatIndex      - Flat index (In)
//		ianmltAnimalType - Animal type (In)
//
// Return Value : 0 if animal is compatible, 1 otherwise
// 
//---------------------------------------------------------------------------------------
int AreNeighborsEnemies(Flat imatFlBuilding[][FLATS_PER_FLOOR], int inFloorIndex,
						int inFlatIndex, AnimalType ianmltAnimalType)
{
	// Variable definition
	int nFloorIndex;
	int nFlatIndex;
	int nNewFloorIndex;
	int nNewFlatIndex;

	// Code section

	// Adjusting correct enemy.
	switch (ianmltAnimalType)
	{
	case CHICKEN:
		ianmltAnimalType = ANT;
		break;
	case ANT:
		ianmltAnimalType = CHICKEN;
		break;
	case CAT:
		ianmltAnimalType = PIG;
		break;
	case PIG:
		ianmltAnimalType = CAT;
		break;
	default:
		return (0);
	}
	// Iterating through every neighboring flat for given indexes.
	for (nFloorIndex = -1, nNewFloorIndex = inFloorIndex + nFloorIndex;
		 nFloorIndex <= 1 && nNewFloorIndex < FLOORS;
		 nFloorIndex++, nNewFloorIndex = inFloorIndex + nFloorIndex)
	{
		// Iterated index must be in range.
		if (nNewFloorIndex < 0);
		else for (nFlatIndex = -1, nNewFlatIndex = inFlatIndex + nFlatIndex;
				  nFlatIndex <= 1 && nNewFlatIndex < FLATS_PER_FLOOR;
				  nFlatIndex++, nNewFlatIndex = inFlatIndex + nFlatIndex)
		{
			// Iterated index must be in range.
			if (nNewFlatIndex < 0);
			else if (imatFlBuilding[nNewFloorIndex][nNewFlatIndex].anmltAnimalType ==
					 ianmltAnimalType)
			{
				return (1);
			}
		}
		return (0);
	}
}