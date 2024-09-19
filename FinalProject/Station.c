#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Station.h"

int isSameStation(const Station* pStat1, const Station* pStat2)
{
	if (!pStat1 || !pStat2)
		return 0;
	if (strcmp(pStat1->codeStation, pStat2->codeStation) == 0)
		return 1;

	return 0;

}


int isStationCode(const Station* pStat, const char* code)
{
	if (!pStat)
		return 0;
	if (strcmp(pStat->codeStation, code) == 0)
		return 1;

	return 0;

}


int initStationNoCodeNoWorker(Station* pStat)
{
	if (getStationName(pStat)==0)
		return 0;

    getHeightOfStation(pStat);

	return 1;

}


int getStationName(Station* pStat)
{
	char temp[255];
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;

	while (count == 0)
	{
		printf("Enter Station name\n");
		myGets(temp, MAX_STR_LEN);
		wordsArray = splitCharsToWords(temp, &count, &totalLength);
	}
	pStat->name = allocateRightLength(wordsArray, count, totalLength);
	if (!pStat->name)
		return 0;

	if (count == 1)
		changeNameOneWord(pStat->name, wordsArray[0]);
	else
		changeName(pStat->name, count, wordsArray);

	//clean temp data
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);
	return 1;

}

void getStationCode(char* code)
{
	char temp[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter Station code  - %d UPPER CASE letters\t", STATION_COD_LENGTH);
		myGets(temp, MAX_STR_LEN);
		if (strlen(temp) != STATION_COD_LENGTH)
		{
			printf("code should be %d letters\n", STATION_COD_LENGTH);
			ok = 0;
		}
		else {
			for (int i = 0; i < STATION_COD_LENGTH; i++)
			{
				if (isupper(temp[i]) == 0)
				{
					printf("Need to be upper case letter\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);

	strcpy(code, temp);

}

int	 saveStationToFile(const Station* pStation, FILE* fp)
{
	if (fp == NULL || pStation == NULL) {
		printf("Error: File pointer is NULL or station\n");
		return 0;
	}

	// Save Station data
	fprintf(fp, "%s\n", pStation->name);
	fprintf(fp, "%s\n", pStation->codeStation);
	fprintf(fp, "%d\n", pStation->heightOfStation);
	fprintf(fp, "%d\n", pStation->workerOfStation->serialNum);

	return 1;

}

int	 loadStationFromFile(Station* pStation,Worker* workerArr,int workerCount, FILE* fp)
{
	if (fp == NULL || pStation == NULL) {
		printf("Error: File pointer is NULL or station\n");
		return 0;
	}

	char tempStationName[MAX_STR_LEN];
	myGetsF(tempStationName, MAX_STR_LEN, fp);
	pStation->name = getDynStr(tempStationName);

	myGetsF(pStation->codeStation, MAX_STR_LEN, fp);

	fscanf(fp, "%d", &pStation->heightOfStation);
	fgetc(fp);
	int tempSN;
	fscanf(fp, "%d", &tempSN);
	fgetc(fp);
	
	// Allocate memory for Worker
	pStation->workerOfStation = (Worker*)malloc(sizeof(Worker));
	if (pStation->workerOfStation == NULL) {
		printf("Error: Memory allocation failed\n");
		free(pStation->name);
		return 0;
	}
	// Find right worker from Workers array
	pStation->workerOfStation = findWorkerBySN(workerArr, workerCount, tempSN);
	if (pStation->workerOfStation == NULL) {
		printf("Error: Worker information is not correct , there is no worker with this SN %d\n",tempSN);
		freeStation(pStation);
		return 0;
	}


	return 1;
}

int	  saveStationToBinaryFile(const Station* pStat, FILE* fp)
{
	if (fp == NULL || pStat == NULL) {
		printf("Error: File pointer is NULL or station\n");
		return 0;
	}
	if (!writeStringToFile(pStat->name, fp, "Error write station name to binary file\n"))
		return 0;
	if (!writeCharsToFile(pStat->codeStation, STATION_COD_LENGTH, fp, "Error write station code to binary file\n"))
		return 0;
	if (!writeIntToFile(pStat->heightOfStation, fp, "Error write station height to binary file\n"))
		return 0;
	if (!writeIntToFile(pStat->workerOfStation->serialNum, fp, "Error write SN of station worker to binary file\n"))
		return 0;

	return 1;
}

int	  loadStationFromBinaryFile(Station* pStat, Worker* workerArr, int workerCount, FILE* fp)
{
	if (fp == NULL || pStat == NULL) {
		printf("Error: File pointer is NULL or station\n");
		return 0;
	}
	pStat->name = readStringFromFile(fp, "Error read station name from binary file\n");
	if (!pStat->name)
		return 0;
	if (!readCharsFromFile(pStat->codeStation, STATION_COD_LENGTH, fp, "Error read station code from binary file\n"))
	{
		free(pStat->name);
		return 0;
	}
	if (!readIntFromFile(&pStat->heightOfStation,fp,"Error read station height from binary file\n"))
	{
		free(pStat->name);
		return 0;
	}
	int tempSN;
	if (!readIntFromFile(&tempSN ,fp, "Error read SN of station worker from binary file\n"))
	{
		free(pStat->name);
		return 0;
	}

	// Allocate memory for Worker
	pStat->workerOfStation = (Worker*)malloc(sizeof(Worker));
	if (pStat->workerOfStation == NULL) {
		printf("Error: Memory allocation failed\n");
		free(pStat->name);
		return 0;
	}
	// Find right worker from Workers array
	pStat->workerOfStation = findWorkerBySN(workerArr, workerCount, tempSN);
	if (pStat->workerOfStation == NULL) {
		printf("Error: Worker information is not correct , there is no worker with this SN %d\n", tempSN);
		freeStation(pStat);
		return 0;
	}
	pStat->codeStation[STATION_COD_LENGTH] = '\0';

	return 1;
}


void printStation(const Station* pStat)
{
	printf("Station name is: %-10s\t", pStat->name);
	printf("Height: %d  Code: %s\n",pStat->heightOfStation, pStat->codeStation);
	printWorker(pStat->workerOfStation);

}


void freeStation(Station* pStat)
{
	if (pStat == NULL) {
		return; // Do nothing if person is NULL
	}
	free(pStat->name);
	freeWorker(pStat->workerOfStation);
	free(pStat);
}




void getHeightOfStation(Station* pStat)
{
	int tempHeight;
	int logicHeight = 1;
	do {
		printf("What is the height of this station in meters (1000 - 6000) ->\n");
		scanf("%d", &tempHeight);
		if (tempHeight >= MIN_HEIGHT && tempHeight <= MAX_HEIGHT)
			logicHeight = 0;
	} while (logicHeight);
	pStat->heightOfStation = tempHeight;
	
}




