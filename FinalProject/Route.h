#ifndef __ROUTE__
#define __ROUTE__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StationManager.h" 



#define ROUTE_DIST_TAV 'm'

typedef enum { eGreen, eBlue, eRed,eBlack, eNofRouteTypes } eRouteType;
static const char* routeTypeStr[eNofRouteTypes] = {"Green", "Blue", "Red","Black"};

typedef struct
{
	char startCode[STATION_COD_LENGTH + 1];
	char endCode[STATION_COD_LENGTH + 1];
	int routeLength;
	eRouteType routeType;
} Route;

void        initRoute(Route* pRoute, StationManager* pManager);
Station*    setStationToRide(const StationManager* pMap, const char* msg);
const char* getRouteTypeStr(int type);
eRouteType	getRouteType();
int         isRouteBeginFromStation(const Route* proute, const char* StationCode);
int         isRouteFinishInStation(const Route* proute, const char* StationCode);
void        printRoute(const Route* pRoute); 
int         getRouteLength(Route* pRoute,const StationManager* pMap);
int         getRouteElevation(Route* pRoute, const StationManager* pMap);
int		    compareRoutesByLength(const void* route1, const void* route2); 

int		    loadRouteFromFile(Route* pRoute, const StationManager* pManager,  FILE* fp); 
int		    saveRouteToFile(const Route* pRoute, FILE* fp); 

int	        saveRouteToBinaryFile(const Route* pRoute, FILE* fp);
int	        loadRouteFromBinaryFile(Route* pRoute, const StationManager* pManager, FILE* fp);

#endif