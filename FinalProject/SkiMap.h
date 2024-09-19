#ifndef __SKI_MAP__
#define __SKI_MAP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Route.h"




typedef struct
{        
	StationManager* manager;
	Route** routeArr;      
	int  routeCount; 
} SkiMap;

int       initSkiMap(SkiMap* pSkiMap);
int       addRoute(SkiMap* pSkiMap, StationManager* pManager);
void      printRoutes(const SkiMap* pSkiMap);
void      printSkiMap(const SkiMap* pMap, const StationManager* pManager);

void      freeSkiMap(SkiMap* pSkiMap);
void      freeRouteArr(Route** routeArr);

int		  saveSkiMapToFile(const SkiMap* pSkiMap, FILE* fp);
int		  loadSkiMapFromFile(SkiMap* pSkiMap, Worker* workerArr, int workerCount, FILE* fp);

int	      saveSkiMapToBinaryFile(const SkiMap* pSkiMap, FILE* fp);
int	      loadSkiMapFromBinaryFile(SkiMap* pSkiMap, Worker* workerArr, int workerCount, FILE* fp);



#endif
