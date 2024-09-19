#ifndef __STATION_MANAGER__
#define __STATION_MANAGER__

#include "GeneralList.h"
#include "Station.h"




typedef struct
{
	LIST		stationList;
}StationManager;

int		   initManager(StationManager* pManager);
int		   addStation(StationManager* pManager, Worker* pWorker);
int		   initStationNoWorker(Station* pStation, StationManager* pManager);
int		   insertNewStationToList(LIST* pProductList, Station* pStation);
Station*   getLastStation(const StationManager* manager);
Station*   findStationByCode(const StationManager* pManager, const char* code);
int		   checkUniqueCode(const char* code, const StationManager* pManager);
void	   printStations(const StationManager* pManager);
int		   getStationCount(const StationManager* pManager);

int		   saveManagerToFile(const StationManager* pManager, FILE* fp);
int		   loadManagerFromFile(StationManager* pManager, Worker* workerArr, int workerCount, FILE* fp);

int	       saveManagerToBinaryFile(const StationManager* pManager, FILE* fp);
int	       loadManagerFromBinaryFile(StationManager* pManager, Worker* workerArr, int workerCount, FILE* fp);

void	   freeManager(StationManager* pManager);

#endif
