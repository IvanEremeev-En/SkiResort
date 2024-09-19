#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SkiMap.h"

int initSkiMap(SkiMap* pSkiMap)
{
     if (pSkiMap == NULL) 
            return 0;
     pSkiMap->manager = (StationManager*)malloc(sizeof(StationManager));
     if (!initManager(pSkiMap->manager))
         return 0;
     pSkiMap->routeArr = (Route**)malloc(1 * sizeof(Route));
     if (pSkiMap->routeArr == NULL) {
         // Allocation failed, return NULL
         return 0;
     }
     
     pSkiMap->routeCount = 0;
     
        return 1;

}


int addRoute(SkiMap* pSkiMap, StationManager* pManager)
{
    if (pSkiMap == NULL) {
        return -1;
    }
    if (pManager == NULL) {
        return -1;
    }

    if (getStationCount(pManager) < 2)
    {
        printf("There are not enough station to set a route, add more stations first!\n");
        return 0;
    }

    Route* pRoute = (Route*)calloc(1, sizeof(Route));
    if (!pRoute)
        return 0;

    printStations(pManager);
    initRoute(pRoute, pManager);

    pSkiMap->routeArr = (Route**)realloc(pSkiMap->routeArr, (pSkiMap->routeCount + 1) * sizeof(Route*));
    if (!pSkiMap->routeArr)
    {
        free(pRoute);
        return 0;
    }

    pSkiMap->routeArr[pSkiMap->routeCount] = pRoute;
    pSkiMap->routeCount++;

    return 1;

}


void printRoutes(const SkiMap* pSkiMap)
{
    // Check if the SkiMap pointer is NULL
    if (pSkiMap == NULL) {
        printf("Error: pSkiMap pointer is NULL\n");
        return;
    }

    // Print title
    printf("List of routes:\n");

    // Loop to print all routes
    for (int i = 0; i < pSkiMap->routeCount; ++i) {
        printf("Route %d:\n", i + 1);
        // Call the function to print the route
        printRoute(pSkiMap->routeArr[i]);
    }

}



 

    void   printSkiMap(const SkiMap * pMap, const StationManager * pManager)
{
    
    if (pMap == NULL) {
        printf("Error: pMap pointer is NULL\n");
        return;
    }
    
    printf("Stations:\n");
    printStations(pManager);

    printf("Routes:\n");
    printRoutes(pMap);

    

}


void freeSkiMap(SkiMap* pSkiMap)
{
    // Check if the SkiMap pointer is NULL
    if (pSkiMap == NULL) {
        printf("Error: pSkiMap pointer is NULL\n");
        return;
    }

    // Free memory allocated for Manager
    if (pSkiMap->manager != NULL) {
        freeManager(pSkiMap->manager);
        pSkiMap->manager = NULL;
    }

    // Free memory allocated for routeArr
    if (pSkiMap->routeArr != NULL) {
        // Call function to free memory for each route
        freeRouteArr(pSkiMap->routeArr);
        free(pSkiMap->routeArr);
        pSkiMap->routeArr = NULL;
    }

    

    // Reset counter
    pSkiMap->routeCount = 0;
    free(pSkiMap);
}



void      freeRouteArr(Route** routeArr)
{
    // Check if the routeArr pointer is NULL
    if (routeArr == NULL) {
        printf("Error: routeArr pointer is NULL\n");
        return;
    }

    // Iterate through the array and free memory for each route
    for (int i = 0; routeArr[i] != NULL; ++i) {
        free(routeArr[i]);
        routeArr[i] = NULL;
    }
    

}

int		saveSkiMapToFile(const SkiMap* pSkiMap, FILE* fp)
{
    if (pSkiMap == NULL || fp == NULL)
        return 0;
    if (!saveManagerToFile(pSkiMap->manager, fp))
        return 0;
    fprintf(fp, "%d\n", pSkiMap->routeCount);
    if (pSkiMap->routeCount > 0)
    {
        for (int i = 0; i < pSkiMap->routeCount; i++)
        {
            if (!saveRouteToFile(pSkiMap->routeArr[i], fp))
                return 0;
        }
    }

    return 1;
}

int		loadSkiMapFromFile(SkiMap* pSkiMap,Worker* workerArr, int workerCount, FILE* fp)
{
    if (pSkiMap == NULL || fp == NULL)
        return 0;
    pSkiMap->manager = (StationManager*)malloc(sizeof(StationManager));
    if (!loadManagerFromFile(pSkiMap->manager, workerArr, workerCount, fp))
    {
        freeManager(pSkiMap->manager);
        return 0;
    }
    fscanf(fp, "%d", &pSkiMap->routeCount);
    fgetc(fp);
    if (pSkiMap->routeCount > 0)
    {
        pSkiMap->routeArr = (Route**)malloc(pSkiMap->routeCount*sizeof(Route*));
        if (!pSkiMap->routeArr)
        {
            free(pSkiMap->routeArr);
            return 0;
        }
        Route* tempRoute;
        for (int i = 0; i < pSkiMap->routeCount; i++)
        {
            tempRoute= (Route*)calloc(1, sizeof(Route));
            if (!tempRoute)
            {
                free(pSkiMap->routeArr);
                return 0;
            }
            if (!loadRouteFromFile(tempRoute, pSkiMap->manager, fp))
            {
                free(pSkiMap->routeArr);
                free(tempRoute);
                return 0;
            }
            pSkiMap->routeArr[i] = tempRoute;
        }
    }
    else
    {
        pSkiMap->routeArr = (Route**)malloc(sizeof(Route*));
        if (!pSkiMap->routeArr)
            return 0;
    }
        

    return 1;
}

int	  saveSkiMapToBinaryFile(const SkiMap* pSkiMap, FILE* fp)
{
    if (pSkiMap == NULL || fp == NULL) {
        return 0; // Invalid input
    }

    if (!saveManagerToBinaryFile(pSkiMap->manager, fp))
        return 0;
    if (!writeIntToFile(pSkiMap->routeCount, fp, "Error write routes count to binary file\n"))
        return 0;
    if (pSkiMap->routeCount > 0)
    {
        for (int i = 0; i < pSkiMap->routeCount; i++)
        {
            if (!saveRouteToBinaryFile(pSkiMap->routeArr[i], fp))
                return 0;
        }
    }

    
    return 1; // Successfully saved SkiMap to binary file


}

int	  loadSkiMapFromBinaryFile(SkiMap* pSkiMap, Worker* workerArr, int workerCount, FILE* fp)
{
    if (pSkiMap == NULL || fp == NULL) {
        return 0; // Invalid input
    }

    pSkiMap->manager = (StationManager*)malloc(sizeof(StationManager));
    if (!loadManagerFromBinaryFile(pSkiMap->manager, workerArr, workerCount, fp))
    {
        freeManager(pSkiMap->manager);
        return 0;
    }

    if (!readIntFromFile(&pSkiMap->routeCount, fp, "Error read routes count from binary file\n"))
        return 0;

    if (pSkiMap->routeCount > 0)
    {
        pSkiMap->routeArr = (Route**)malloc(pSkiMap->routeCount * sizeof(Route*));
        if (!pSkiMap->routeArr)
        {
            free(pSkiMap->routeArr);
            return 0;
        }
        Route* tempRoute;
        for (int i = 0; i < pSkiMap->routeCount; i++)
        {
            tempRoute = (Route*)calloc(1, sizeof(Route));
            if (!tempRoute)
            {
                free(pSkiMap->routeArr);
                free(tempRoute);
                return 0;
            }
            if (!loadRouteFromBinaryFile(tempRoute, pSkiMap->manager, fp))
            {
                free(pSkiMap->routeArr);
                free(tempRoute);
                return 0;
            }
            pSkiMap->routeArr[i] = tempRoute;
        }
    }else
    {
        pSkiMap->routeArr = (Route**)malloc(sizeof(Route*));
        if (!pSkiMap->routeArr)
            return 0;
    }

    return 1; // Successfully loaded SkiMap from binary file
    
}





