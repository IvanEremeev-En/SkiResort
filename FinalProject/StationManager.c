#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "StationManager.h"


int		initManager(StationManager* pManager)
{
	
	if (!L_init(&pManager->stationList))
		return 0;

	return 1;

}

int		addStation(StationManager* pManager, Worker* pWorker)
{
	Station* pStation = (Station*)calloc(1, sizeof(Station));
	if (!pStation)
		return 0;

	if (!initStationNoWorker(pStation, pManager))
	{
		freeStation(pStation); //will free also pStation
		return 0;
	}
	pStation->workerOfStation = pWorker;

	return insertNewStationToList(&pManager->stationList, pStation);

}

int		initStationNoWorker(Station* pStation, StationManager* pManager)
{
	while (1)
	{
		getStationCode(pStation->codeStation);
		if (!checkUniqueCode(pStation->codeStation, pManager))
		{
			return 0;
			printf("This code already in use - enter a different code for new station ->\n");
		}
		break;
	}

	return initStationNoCodeNoWorker(pStation);

}

int		insertNewStationToList(LIST* pProductList, Station* pStation)
{
	NODE* pN = pProductList->head.next; //first Node
	NODE* pPrevNode = &pProductList->head;
	Station* pTemp;
	int compRes;
	while (pN != NULL)
	{
		pTemp = (Station*)pN->key;
		// comper by Height of station
		compRes = pTemp->heightOfStation - pStation->heightOfStation;
		
		if (compRes > 0) {//found a place for new item, the next one bigger	
			if (!L_insert(pPrevNode, pStation))
				return 0;
			return 1;
		}
		pPrevNode = pN;
		pN = pN->next;
	}
	//insert at end
	if (!L_insert(pPrevNode, pStation))
		return 0;
	return 1;


}

Station* getLastStation(const StationManager* manager) {
	// If the list is empty, return NULL
	if (&manager->stationList.head == NULL) {
		return NULL;
	}
	// Traverse the list to find the last station
	const NODE* current = &manager->stationList.head;
	while (current->next != NULL) {
		current = current->next;
	}
	
	return current->key;
}



Station* findStationByCode(const StationManager* pManager, const char* code)
{
	NODE* pN = pManager->stationList.head.next; //first Node
	if (!pN)
		return NULL;

	Station* pTemp;
	int compRes;
	while (pN != NULL)
	{
		pTemp = (Station*)pN->key;
		compRes = strcmp(pTemp->codeStation, code);
		if (compRes == 0)
			return pTemp;
		if (compRes > 1) //not fount
			return NULL;
		pN = pN->next;
	}

	return NULL;

}

int		checkUniqueCode(const char* code, const StationManager* pManager)
{
	if (getStationCount(pManager) == 0)
		return 1;
	else
	{
		Station* tempStation = findStationByCode(pManager, code);

		if (tempStation != NULL)
			return 0;

		return 1;
	}
}

void	printStations(const StationManager* pManager)
{
	int count = getStationCount(pManager);
	printf("There are % d stations in our Resort:\n", count);
	L_print(&pManager->stationList, printStation);

}


int		getStationCount(const StationManager* pManager)
{
	int count = 0;
	const NODE* pN = &pManager->stationList.head; //first Node

	while (pN->next != NULL)
	{
		count++;
		pN = pN->next;
	}
	return count;

}


int		saveManagerToFile(const StationManager* pManager, FILE* fp)
{
	
	if (fp == NULL || pManager == NULL) {
		return 0;
	}

	int count = getStationCount(pManager);
	fprintf(fp, "%d\n", count);
	if (count > 0)
	{
		NODE* pN = pManager->stationList.head.next; //first Node

		Station* pTemp;
		/*while (pN->next != NULL)*/
		for(int j=0 ; j<count ; j++)
		{
			pTemp = (Station*)pN->key;
			if (!saveStationToFile(pTemp, fp))
			{
				printf("Error write station to file\n");
				return 0;
			}
			pN = pN->next;
		}

	}

	return 1;

}



int		loadManagerFromFile(StationManager* pManager,Worker* workerArr, int workerCount, FILE* fp)
{
	if (fp == NULL || pManager == NULL) {
		return 0;
	}

	L_init(&pManager->stationList);
	int count;
	fscanf(fp, "%d", &count);
	fgetc(fp);

	Station* pStation;
	for (int i = 0; i < count; i++)
	{
		pStation = (Station*)calloc(1, sizeof(Station));
		if (!pStation)
			break;
		if (!loadStationFromFile(pStation,workerArr,workerCount, fp))
		{
			printf("Error loading station %d from file\n",i+1);
			freeStation(pStation);
			return 0;
		}
		if (!insertNewStationToList(&pManager->stationList, pStation))
		{
			freeStation(pStation);
			return 0;
		}
			
	}
	
	return 1;

}

int saveManagerToBinaryFile(const StationManager* pManager, FILE* fp) 
{
	if (fp == NULL || pManager == NULL) {
		return 0;
	}
	int stationCount = getStationCount(pManager);
	if (!writeIntToFile(stationCount, fp, "Error write station count to binary file\n"))
		return 0;
	if (stationCount > 0)
	{
		NODE* pN = pManager->stationList.head.next; //first Node

		Station* pTemp;
		for(int j=0; j<stationCount ;j++)
		{
			pTemp = (Station*)pN->key;
			if (!saveStationToBinaryFile(pTemp, fp))
			{
				printf("Error write station to file\n");
				return 0;
			}
			pN = pN->next;
		}
	}


	return 1;
}

int loadManagerFromBinaryFile(StationManager* pManager, Worker* workerArr, int workerCount, FILE* fp) 
{
	if (fp == NULL || pManager == NULL) {
		return 0;
	}
	int stationCount;
	if (!readIntFromFile(&stationCount, fp, "Error read station count from binary file\n"))
		return 0;
	L_init(&pManager->stationList);

	Station* pStation;
	for (int i = 0; i < stationCount; i++)
	{
		pStation = (Station*)calloc(1, sizeof(Station));
		if (!pStation)
			break;
		if (!loadStationFromBinaryFile(pStation, workerArr, workerCount, fp))
		{
			printf("Error loading station %d from binary file\n", i + 1);
			freeStation(pStation);
			return 0;
		}
		if (!insertNewStationToList(&pManager->stationList, pStation))
		{
			freeStation(pStation);
			return 0;
		}
			
	}

	return 1;
}


void	freeManager(StationManager* pManager)
{
	if (pManager == NULL) {
		return; // Do nothing if person is NULL
	}
	L_free(&pManager->stationList, freeStation);

}






