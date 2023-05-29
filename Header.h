// Header.h

#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>

#define FLOORS 10
#define FLATS_PER_FLOOR 7
#define FILE_NAME "Tenants.dat"

// Enum declaration
typedef enum { NONE, CHICKEN, ANT, CAT, PIG, DOVE } AnimalType;

// Struct declaration
typedef struct {
	int nNeighbors;
	int nMaxNeighbors;
} Animal, *AnimalPtr;

typedef struct {
	AnimalType anmltAnimalType;
	AnimalPtr anmlAnimal;
} Flat;