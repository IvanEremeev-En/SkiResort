#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Route.h"


void initRoute(Route* pRoute, StationManager* pManager)
{
	Station* pStartStation = setStationToRide(pManager, "Enter code of Start station: ");
	strcpy(pRoute->startCode, pStartStation->codeStation);
	int same;
	Station* pEndStation;
	do {
		pEndStation = setStationToRide(pManager, "Enter code of end station:");
		same = isSameStation(pStartStation,pEndStation);
		if (same)
			printf("Same start and finish station\n");
	} while (same);
	strcpy(pRoute->endCode, pEndStation->codeStation);
	pRoute->routeType = getRouteType();
	pRoute->routeLength = getRouteLength(pRoute, pManager);

}


Station* setStationToRide(const StationManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Station* tempStation;
	do
	{
		printf("%s\t", msg);
		getStationCode(code);
		tempStation = findStationByCode(pManager, code);
		if (tempStation == NULL)
			printf("No Station with this code - try again\n");
	} while (tempStation == NULL);

	return tempStation;

}

int		    compareRoutesByLength(const void* route1, const void* route2)
{
	const Route* pFlight1 = *(const Route**)route1;
	const Route* pFlight2 = *(const Route**)route2;
	return pFlight1->routeLength - pFlight2->routeLength;
}

eRouteType getRouteType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofRouteTypes; i++)
			printf("%d for %s\n", i, routeTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofRouteTypes);
	getchar();
	return (eRouteType)option;

}

const char* getRouteTypeStr(int type)
{
	if (type < 0 || type >= eNofRouteTypes)
		return NULL;
	return routeTypeStr[type];

}


int isRouteBeginFromStation(const Route* pRoute, const char* StationCode)
{
	if (strcmp(pRoute->startCode, StationCode) == 0)
		return 1;

	return 0;

}


int isRouteFinishInStation(const Route* pRoute, const char* StationCode)
{
	if (strcmp(pRoute->endCode, StationCode) == 0)
		return 1;

	return 0;

}




int getRouteLength(Route* pRoute,const StationManager* pManager)
{
	int tempDis;
	int rightLength = 0;
	do {
         printf("What is the length of the route in %c\n", ROUTE_DIST_TAV);
	     scanf("%d", &tempDis);
		 int elevation = getRouteElevation(pRoute, pManager);
       	 if (elevation < tempDis) // Check if user input is logic for distance , it has to be longer than the elevation bitween route stations
		     rightLength = 0;
	     else
		     printf("Wrong distance, this route must by longer that %d meters ! \n",elevation);
	} while (rightLength);
	return tempDis;
	
}


int getRouteElevation(Route* pRoute, const StationManager* pManager)
{
	Station* temp1;
	Station* temp2;
	temp1 = findStationByCode(pManager, pRoute->startCode);
	if (!temp1)
	{
		printf("Route start Station ERROR!\n");
		return 0;
	}
	temp2 = findStationByCode(pManager, pRoute->endCode);
	if (!temp2)
	{
		printf("Route end Station ERROR!\n");
		return 0;
	}
	return temp1->heightOfStation - temp2->heightOfStation;

}



int		saveRouteToFile(const Route* pRoute, FILE* fp)
{
	if (pRoute == NULL || fp == NULL) {
		return 0; // Invalid input
	}
	fprintf(fp, "%s\n", pRoute->startCode);
	fprintf(fp, "%s\n", pRoute->endCode);
	fprintf(fp, "%d\n", pRoute->routeLength);
	fprintf(fp, "%d\n", pRoute->routeType);
	return 1;
}

int		loadRouteFromFile(Route* pRoute, const StationManager* pManager, FILE* fp)
{
	if (pRoute == NULL || fp == NULL || pManager == NULL) {
		return 0; // Invalid input
	}
	
	myGetsF(pRoute->startCode, MAX_STR_LEN, fp);
	

	if (findStationByCode(pManager, pRoute->startCode) == NULL)
	{
		printf("Station with code: %s  not in resort manager\n", pRoute->startCode);
		return 0;
	}

	myGetsF(pRoute->endCode, MAX_STR_LEN, fp);

	if (findStationByCode(pManager, pRoute->endCode) == NULL)
	{
		printf("Station with code: %s  not in resort manager\n", pRoute->endCode);
		return 0;
	}

	fscanf(fp, "%d", &pRoute->routeLength);
	fgetc(fp);
	fscanf(fp, "%d", &pRoute->routeType);
	fgetc(fp);
	
	return 1;
}

int	  saveRouteToBinaryFile(const Route* pRoute, FILE* fp)
{
	if (pRoute == NULL || fp == NULL) {
		return 0; // Invalid input
	}

	// Write startCode, endCode, routeLength, and routeType to the file
	if (!writeCharsToFile(pRoute->startCode, STATION_COD_LENGTH,fp,"Error write code of start station  to binary file\n"))
		return 0;
	if (!writeCharsToFile(pRoute->endCode, STATION_COD_LENGTH, fp, "Error write code of end station  to binary file\n"))
		return 0;
	if (!writeIntToFile(pRoute->routeLength, fp, "Error write length of the route to binary file\n"))
	    return 0;
	if (!writeIntToFile(pRoute->routeType, fp, "Error write type of the route to binary file\n"))
		return 0;
	
	return 1; // Successfully saved Route to file
}

int	  loadRouteFromBinaryFile(Route* pRoute, const StationManager* pManager, FILE* fp)
{
	if (pRoute == NULL || fp == NULL || pManager == NULL) {
		return 0; // Invalid input
	}

	if (!readCharsFromFile(pRoute->startCode, STATION_COD_LENGTH, fp, "Error read start station code from binary file\n"))
		return 0;

	if (findStationByCode(pManager, pRoute->startCode) == NULL)
	{
		printf("Station with code: %s  not in resort manager\n", pRoute->startCode);
		return 0;
	}

	if (!readCharsFromFile(pRoute->endCode, STATION_COD_LENGTH, fp, "Error read end station code from binary file\n"))
		return 0;

	if (findStationByCode(pManager, pRoute->endCode) == NULL)
	{
		printf("Station with code: %s  not in resort manager\n", pRoute->endCode);
		return 0;
	}

	if (!readIntFromFile(&pRoute->routeLength, fp, "Error read length of route from binary file\n"))
		return 0;
	int routeType;
	if (!readIntFromFile(&routeType, fp, "Error read length of route from binary file\n"))
		return 0;
	pRoute->routeType = routeType;

	pRoute->startCode[STATION_COD_LENGTH] = '\0';
	pRoute->endCode[STATION_COD_LENGTH] = '\0';

	return 1; // Successfully loaded Route from file
}


void printRoute(const Route* pRoute)
{
	// Check if the route pointer is NULL
	if (pRoute == NULL) {
		printf("Error: route pointer is NULL\n");
		return;
	}

	// Print route details, including color
	printf("Route starts at station %s and ends at station %s with a length of %d meters. Route color: %s\n",
		pRoute->startCode, pRoute->endCode, pRoute->routeLength, routeTypeStr[pRoute->routeType]);

}


