#ifndef __STATION__
#define __STATION__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Worker.h"

#define MIN_HEIGHT 1000
#define MAX_HEIGHT 6000
#define STATION_COD_LENGTH 4

typedef struct
{
	char* name;
	Worker* workerOfStation;
	char codeStation[STATION_COD_LENGTH + 1];
	int  heightOfStation;

} Station;



int   isSameStation(const Station* pStat1, const Station* pstat2);
int   isStationCode(const Station* pStat, const char* code);
int   initStationNoCodeNoWorker(Station* pStat);
int   getStationName(Station* pStat);
void  getStationCode(char* code);
 
int	  saveStationToFile(const Station* pStation, FILE* fp); 
int	  loadStationFromFile(Station* pStation, Worker* workerArr, int workerCount, FILE* fp);

int	  saveStationToBinaryFile(const Station* pStat, FILE* fp);
int	  loadStationFromBinaryFile(Station* pStat, Worker* workerArr, int workerCount, FILE* fp);

void  printStation(const Station* pStat);
void  freeStation(Station* pStat);
void  getHeightOfStation(Station* pStat);

#endif