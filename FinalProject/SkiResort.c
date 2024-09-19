#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SkiResort.h"

typedef enum { eByNone, eByDate, eBySkiPass, eBySkiType, eNofSortTypes } eSortType;
static const char* sortTypeStr[eNofSortTypes] = { "None", "Date", "SkiPass","SkiType" };


int initSkiResort(SkiResort* pSkiRes, char* name)
{
    if (pSkiRes == NULL) {
        return 0;
    }

    pSkiRes->name = name;
    pSkiRes->theMap = (SkiMap*)malloc(sizeof(SkiMap));
    if (pSkiRes->theMap == NULL) {
        return 0;
    }
    if (!initSkiMap(pSkiRes->theMap))
        return 0;
    // Allocate memory for the Reservation array
    pSkiRes->allReservations = malloc(INITIAL_RESERVATION_CAPACITY * sizeof(Reservation));
    if (pSkiRes->allReservations == NULL) {
        return 0;
    }

    // Allocate memory for the Worker array
    pSkiRes->workerArr = malloc(INITIAL_WORKER_CAPACITY * sizeof(Worker));
    if (pSkiRes->workerArr == NULL) {
        return 0;
    }

    // Allocate memory for the array of client pointers
    pSkiRes->allClients = malloc(INITIAL_CLIENT_CAPACITY * sizeof(Person*));
    if (pSkiRes->allClients == NULL) {
        return 0;
    }

    // Allocate memory for the array of all the people pointers
    pSkiRes->allPeople = malloc(INITIAL_CLIENT_CAPACITY * sizeof(Person*));
    if (pSkiRes->allPeople == NULL) {
        return 0;
    }

    pSkiRes->reservationCount = 0;
    pSkiRes->workerCount = 0;
    pSkiRes->clientCount = 0;
    return 1;
}


int        initStation(SkiResort* pSkiRes)
{
    if (pSkiRes == NULL)
    {
        return 0; 
    }
    printf("Befor you creat your station we need first creat worker that will be in charge there:\n");
    if (!(addWorker(pSkiRes)))
        return 0;
    

    addStation(pSkiRes->theMap->manager, &pSkiRes->workerArr[pSkiRes->workerCount - 1]);
    

    return 1;
}

int        addPerson(SkiResort* pSkiRes)
{
    if (pSkiRes == NULL) {
        return -1; 
    }

    int allPeopleCount = pSkiRes->workerCount + pSkiRes->clientCount;
    pSkiRes->allPeople = (Person**)realloc(pSkiRes->allPeople, (allPeopleCount + 1) * sizeof(Person*));
    if (!pSkiRes->allPeople)
        return 0;
    pSkiRes->allPeople[allPeopleCount] = (Person*)malloc(sizeof(Person));
    initPerson(pSkiRes->allPeople[allPeopleCount], *pSkiRes->allPeople, allPeopleCount);

    // Resize the allClients array to accommodate the new person
    pSkiRes->allClients = (Person**)realloc(pSkiRes->allClients, (pSkiRes->clientCount + 1) * sizeof(Person*));
    if (pSkiRes->allClients == NULL) {
        freePerson(pSkiRes->allPeople[pSkiRes->workerCount + pSkiRes->clientCount]); // Free the allocated memory for the new person
        return 0; // Resizing the array failed
    }

    // Add the new person to the array
    pSkiRes->allClients[pSkiRes->clientCount] = pSkiRes->allPeople[allPeopleCount];

    // Increment the client count
    pSkiRes->clientCount++;

    return 1;
}

int        addReservation(SkiResort* pSkiRes) 
{
    if (pSkiRes == NULL) {
        return 0; 
    }

    Reservation* newReservation = (Reservation*)malloc(sizeof(Reservation));
    addPerson(pSkiRes);
    if (initReservation(newReservation, pSkiRes->allClients[pSkiRes->clientCount-1]) != 1) {
        freeReservation(newReservation); 
        return -2; 
    }

    pSkiRes->allReservations = realloc(pSkiRes->allReservations, (pSkiRes->reservationCount + 1) * sizeof(Reservation));
    if (pSkiRes->allReservations == NULL) {
        freeReservation(newReservation); 
        return -3; 
    }

    pSkiRes->allReservations[pSkiRes->reservationCount] = *newReservation;
    pSkiRes->reservationCount++;

    return 1; 
}



int addWorker(SkiResort* pSkiRes)
{
    if (pSkiRes == NULL) {
        return 0; // SkiResort structure is not valid
    }

    pSkiRes->allPeople = (Person**)realloc(pSkiRes->allPeople, (pSkiRes->workerCount+ pSkiRes->clientCount + 1) * sizeof(Person*));
    if (!pSkiRes->allPeople)
        return 0;
    pSkiRes->allPeople[pSkiRes->workerCount + pSkiRes->clientCount] = (Person*)malloc(sizeof(Person));
    initPerson(pSkiRes->allPeople[pSkiRes->workerCount + pSkiRes->clientCount], *pSkiRes->allPeople, pSkiRes->workerCount + pSkiRes->clientCount);
    printf("Enter the details of the new worker:\n");

    // Allocate memory for the new worker
    Worker* newWorkerEntry = (Worker*)malloc(sizeof(Worker));
    if (newWorkerEntry == NULL) {
        freePerson(pSkiRes->allPeople[pSkiRes->workerCount + pSkiRes->clientCount]); // Free the allocated memory for the new person
        return 0; // Memory allocation failed
    }

    // Initialize the new worker
    if (initWorker(newWorkerEntry, pSkiRes->allPeople[pSkiRes->workerCount + pSkiRes->clientCount], pSkiRes->workerArr, pSkiRes->workerCount) != 1)
    {
        freePerson(pSkiRes->allPeople[pSkiRes->workerCount + pSkiRes->clientCount]); // Free the allocated memory for the new person
        freeWorker(newWorkerEntry); // Free the allocated memory for the new worker
        return 0; // Initialization of the worker failed
    }


    // Assign the new worker to the array
    pSkiRes->workerArr[pSkiRes->workerCount] = *newWorkerEntry;

    // Increment the worker count
    pSkiRes->workerCount++;

    return 1; // Successfully added the new worker
}




void       sortReservations(SkiResort* pSkiRes)
{
    if (pSkiRes == NULL || pSkiRes->allReservations == NULL || pSkiRes->reservationCount <= 0) {
        return; 
    }

    eSortType sortType = showSortMenu(); 
    int(*compare)(const void* air1, const void* air2) = NULL;

    switch (sortType)
    {
    case eByDate:
        compare = compareReservationByStartDate;
        break;
    case eBySkiPass:
        compare = compareReservationBySkiPass;
        break;
    case eBySkiType:
        compare = compareReservationBySkyType;
        break;
    }
    if (compare != NULL)
        qsort(pSkiRes->allReservations, pSkiRes->reservationCount, sizeof(Reservation), compare);

}

int        getProfitOfResort(const SkiResort* pSkiRes)
{
    if (pSkiRes->reservationCount == 0)
        return 0;
    int counter = 0;
    for (int i = 0; i < pSkiRes->reservationCount; i++)
    {
        counter += getCostOfSkiPass(&pSkiRes->allReservations[i]);
    }
    return counter;
}

int  showSortMenu()
{
    int opt;
    printf("Base on what field do you want to sort?\n");
    do {
        for (int i = 1; i < eNofSortTypes; i++)
            printf("Enter %d for %s\n", i, sortTypeStr[i]);
        scanf("%d", &opt);
    } while (opt < 0 || opt >= eNofSortTypes);

    return (eSortType)opt;

}

void       printReservationArr(Reservation* ReservationsArr, int reservationCount)
{
    if (ReservationsArr == NULL || reservationCount <= 0) {
        printf("No reservations to display.\n");
        return;
    }

    printf("List of Reservations:\n");
    for (int i = 0; i < reservationCount; i++) {
        printf("Reservation %d:", i + 1);
        printReservation(&ReservationsArr[i]);
        printf("\n");
    }
}

void       printWorkersArr(Worker* workersArr, int workerCount)
{
    if (workersArr == NULL || workerCount <= 0) {
        printf("No workers to print.\n");
        return;
    }

    printf("Workers:\n");
    for (int i = 0; i < workerCount; i++) {
        printf("Worker %d:\n", i + 1);
        printWorker(&workersArr[i]);
        printf("\n");
    }

}

void       printClientArr(Person** clientsArr, int clientCount)
{
    if (clientsArr == NULL || clientCount <= 0) {
        printf("No clients to display.\n");
        return;
    }

    printf("List of Clients:\n");
    for (int i = 0; i < clientCount; i++) {
        printf("Client %d:\n", i + 1);
        printPerson(clientsArr[i]);
        printf("\n");
    }

}

void       printSkiResort(const SkiResort* pSkiRes)
{
    if (pSkiRes == NULL) {
        printf("Invalid Ski Resort.\n");
        return;
    }

    printf("Ski Resort Information:\n");
    printf("Name: %s\n", pSkiRes->name);
    printf("Number of Reservations: %d\n", pSkiRes->reservationCount);
    printf("Number of Workers: %d\n", pSkiRes->workerCount);
    printf("Number of Clients: %d\n", pSkiRes->clientCount);
    printReservationArr(pSkiRes->allReservations, pSkiRes->reservationCount);
    printf("Total income of the Resort is : %d $\n", getProfitOfResort(pSkiRes));
    printWorkersArr(pSkiRes->workerArr, pSkiRes->workerCount);
    printClientArr(pSkiRes->allClients, pSkiRes->clientCount);

}

void   freeWorkerArr(Worker* WorkerArr, int size)
{
    if (WorkerArr == NULL || size <= 0) {
        return; 
    }

    for (int i = 0; i < size; i++) {
        freeWorker(&WorkerArr[i]); 
    }

    free(WorkerArr); 

}

void   freeClientArr(Person** clientArr, int size)
{
    if (clientArr == NULL || size <= 0) {
        return; 
    }

    for (int i = 0; i < size; i++) {
        freePerson(clientArr[i]); 
    }

    free(clientArr); 

}

void   freeAllPeopleArr(Person** allPeople, int size)
{
    if (allPeople == NULL || size <= 0) {
        return;
    }

    for (int i = 0; i < size; i++) {
        freePerson(allPeople[i]);
    }

    free(allPeople);

}

void   freeSkiResort(SkiResort* pSkiRes)
{
    if (pSkiRes == NULL) {
        return; 
    }

    
    if (pSkiRes->reservationCount > 0) {
        for (int i = 0; i < pSkiRes->reservationCount; i++) {
            freeReservation(&(pSkiRes->allReservations[i]));
        }
        free(pSkiRes->allReservations);
    }

    
    if (pSkiRes->workerArr != NULL) {
        freeWorkerArr(pSkiRes->workerArr, pSkiRes->workerCount);
    }

    
    if (pSkiRes->allClients != NULL) {
        freeClientArr(pSkiRes->allClients, pSkiRes->clientCount);
    }

    if (pSkiRes->allPeople != NULL) {
        freeAllPeopleArr(pSkiRes->allPeople, pSkiRes->clientCount + pSkiRes->workerCount);
    }

    
    if (pSkiRes->name != NULL) {
        free(pSkiRes->name);
    }
    if (pSkiRes->theMap != NULL)
    {
          freeSkiMap(pSkiRes->theMap);
    }
    

   
    free(pSkiRes);

}

int		saveSkiResortToFile(const SkiResort* pResort, FILE* fp)
{
    fprintf(fp, "%s\n", pResort->name);
    fprintf(fp, "%d\n", pResort->workerCount);
    fprintf(fp, "%d\n", pResort->clientCount);
    fprintf(fp, "%d\n", pResort->reservationCount);

    int allPeopleCount = pResort->workerCount + pResort->clientCount;
    if (allPeopleCount > 0)
    {
        for (int i = 0; i < allPeopleCount; i++)
        {
            if (!savePersonToFile(pResort->allPeople[i], fp))
            {
                printf("Error save %d people to file\n", i+1);
                return 0;
            }
        }
    }
    if (pResort->workerCount > 0)
    {
        for (int i = 0; i < pResort->workerCount; i++)
        {
            
            if (!saveWorkerToFile(&pResort->workerArr[i], fp))
            {
                printf("Error save %d worker to file\n", i+1);
                return 0;
            }
        }

    }
    if (pResort->clientCount > 0)
    {
        for (int i = 0; i < pResort->clientCount; i++)
        {
            fprintf(fp, "%d\n", pResort->allClients[i]->id);
           
        }
    }
    
    
    if (pResort->reservationCount > 0)
    {
        for (int i = 0; i < pResort->reservationCount; i++)
        {
            if (!saveReservationToFile(&pResort->allReservations[i], fp))
            {
                printf("Error save %d reservation to file\n", i+1);
                return 0;
            }
        }

    }
    

    if (!saveSkiMapToFile(pResort->theMap, fp))
    {
        printf("Error save Ski-Map to file\n");
        return 0;
    }

    return 1;
}

int		loadSkiResortFromFile(SkiResort* pResort, FILE* fp)
{
    if (pResort == NULL || fp == NULL)
        return 0;

    char tempName[MAX_STR_LEN];
    myGetsF(tempName, MAX_STR_LEN, fp);
    pResort->name = getDynStr(tempName);

    fscanf(fp, "%d", &pResort->workerCount);
    fgetc(fp);
    fscanf(fp, "%d", &pResort->clientCount);
    fgetc(fp);
    fscanf(fp, "%d", &pResort->reservationCount);
    fgetc(fp);

    int allPeopleCount = pResort->workerCount + pResort->clientCount;
    pResort->allPeople = (Person**)malloc(allPeopleCount * sizeof(Person*));
    if (!pResort->allPeople)
    {
        free(pResort->name);
        return 0;
    }
    if (allPeopleCount > 0)
    {
        
        Person* tempPerson;
        for (int i = 0; i < allPeopleCount; i++)
        {
            tempPerson = (Person*)calloc(1, sizeof(Person));
            if (!tempPerson)
            {
                free(pResort->name);
                free(pResort->allPeople);
                return 0;
            }
            if (!loadPersonFromFile(tempPerson, fp))
            {
                /*free(pResort->allPeople);*/
                free(tempPerson);
                return 0;
            }
            pResort->allPeople[i] = tempPerson;
        }
    }
    pResort->workerArr = (Worker*)malloc(pResort->workerCount * sizeof(Worker));
    if (!pResort->workerArr) {
        printf("Memory allocation failed for workerArr\n");
        return 0;
    }
    if (pResort->workerCount >0)
    {
        for (int i = 0; i < pResort->workerCount; i++) 
        {
            if (!loadWorkerFromFile(&pResort->workerArr[i], pResort->allPeople, allPeopleCount, fp))
            {
               free(pResort->workerArr);
               return 0;
            }
        }
    }
    

    // Allocate memory for arrays
    pResort->allClients = (Person**)malloc(pResort->clientCount * sizeof(Person*));
    if (!pResort->allClients) {
        printf("Memory allocation failed for allClients\n");
        return 0;
    }

    if (pResort->clientCount > 0)
    {
        for (int i = 0; i < pResort->clientCount; i++) {
            pResort->allClients[i] = (Person*)malloc(sizeof(Person));
            if (!pResort->allClients[i]) {
                printf("Memory allocation failed for allClients[%d]\n", i);
                // Clean up previously allocated memory
                for (int j = 0; j < i; j++)
                    free(pResort->allClients[j]);
                free(pResort->allClients);
                return 0;
            }
            int tempID;
            fscanf(fp, "%d", &tempID);
            fgetc(fp);
            pResort->allClients[i] = findPerson(pResort->allPeople, tempID, allPeopleCount);
            if (pResort->allClients[i] == NULL)
            {
                printf("Error , there is no people in resort with this ID: %d", tempID);
                freePerson(pResort->allClients[i]);
                return 0;
            }
        }
    }

    

    pResort->allReservations = (Reservation*)malloc(pResort->reservationCount * sizeof(Reservation));
    if (!pResort->allReservations) {
        printf("Memory allocation failed for allReservations\n");
        free(pResort->allClients); // Clean up previously allocated memory
        free(pResort->workerArr);
        return 0;
    }


    // Load allReservations
    if (pResort->reservationCount > 0)
    {
        for (int i = 0; i < pResort->reservationCount; i++) {
            if (!loadReservationFromFile(&pResort->allReservations[i], pResort->allClients, pResort->clientCount, fp))
            {
                printf("Error loading reservation %d from file\n", i+1);
                for (int j = i; j < pResort->reservationCount; j++)
                {
                    freeReservation(&pResort->allReservations[j]);
                }
                pResort->reservationCount = pResort->reservationCount - (pResort->reservationCount - i);
                return 0;
            }
        }
    }
    

    // Load theMap
    pResort->theMap = (SkiMap*)malloc(sizeof(SkiMap));
    if (!pResort->theMap) {
        printf("Memory allocation failed for theMap\n");
        return 0;
    }
    if (!loadSkiMapFromFile(pResort->theMap,pResort->workerArr,pResort->workerCount, fp)) {
        printf("Error loading SkiMap from file\n");
        free(pResort->theMap);
        return 0;
    }

    return 1;

}



int	  saveSkiResortToBinaryFile(const SkiResort* pResort, FILE* fp)
{
    
    if (!writeStringToFile(pResort->name, fp, "Error write ski Resort name to binary file\n"))
        return 0;
    if (!writeIntToFile(pResort->workerCount, fp, "Error write ski resort workers count to binary file\n"))
        return 0;
    if (!writeIntToFile(pResort->clientCount, fp, "Error write ski resort clients count to binary file\n"))
        return 0;
    if (!writeIntToFile(pResort->reservationCount, fp, "Error write ski resort reservations count to binary file\n"))
        return 0;

    int allPeopleCount = pResort->workerCount + pResort->clientCount;
    if (allPeopleCount > 0)
    {
        for (int i = 0; i < allPeopleCount; i++)
        {
            if (!savePersonToBinaryFile(pResort->allPeople[i], fp))
            {
                printf("Error save %d people to file\n", i+1);
                return 0;
            }
        }
    }
    if (pResort->workerCount > 0)
    {
        for (int i = 0; i < pResort->workerCount; i++)
        {
            if (!saveWorkerToBinaryFile(&pResort->workerArr[i], fp))
            {
                printf("Error save %d worker to file\n", i+1);
                return 0;
            }
        }

    }
    if (pResort->clientCount > 0)
    {
        for (int i = 0; i < pResort->clientCount; i++)
        {
            if (!writeIntToFile(pResort->allClients[i]->id, fp, "Error write client ID to binary file\n"))
                return 0;
            
        }
    }


    if (pResort->reservationCount > 0)
    {
        for (int i = 0; i < pResort->reservationCount; i++)
        {
            if (!saveReservationToBinaryFile(&pResort->allReservations[i], fp))
            {
                printf("Error save %d reservation to file\n", i+1);
                return 0;
            }
        }

    }


    if (!saveSkiMapToBinaryFile(pResort->theMap, fp))
    {
        printf("Error save Ski-Map to file\n");
        return 0;
    }

    
    return 1; // Success

}

int	  loadSkiResortFromBinaryFile(SkiResort* pResort, FILE* fp)
{
    pResort->name = readStringFromFile(fp, "Error read sri resort from binary file\n");
    if (pResort->name == NULL)
        return 0;
    if (!readIntFromFile(&pResort->workerCount, fp, "Error write ski resort workers count to binary file\n"))
        return 0;
    if (!readIntFromFile(&pResort->clientCount, fp, "Error write ski resort clients count to binary file\n"))
        return 0;
    if (!readIntFromFile(&pResort->reservationCount, fp, "Error write ski resort reservations count to binary file\n"))
        return 0;

    int allPeopleCount = pResort->workerCount + pResort->clientCount;
    pResort->allPeople = (Person**)malloc(allPeopleCount * sizeof(Person*));
    if (!pResort->allPeople)
    {
        free(pResort->name);
        return 0;
    }
    if (allPeopleCount > 0)
    {

        Person* tempPerson;
        for (int i = 0; i < allPeopleCount; i++)
        {
            tempPerson = (Person*)calloc(1, sizeof(Person));
            if (!tempPerson)
            {
                free(pResort->name);
                free(pResort->allPeople);
                return 0;
            }
            if (!loadPersonFromBinaryFile(tempPerson, fp))
            {
                free(tempPerson);
                return 0;
            }
            pResort->allPeople[i] = tempPerson;
        }
    }
    pResort->workerArr = (Worker*)malloc(pResort->workerCount * sizeof(Worker));
    if (!pResort->workerArr) {
        printf("Memory allocation failed for workerArr\n");
        return 0;
    }
    if (pResort->workerCount > 0)
    {
        for (int i = 0; i < pResort->workerCount; i++)
        {
            if (!loadWorkerFromBinaryFile(&pResort->workerArr[i], pResort->allPeople, allPeopleCount, fp))
            {
                free(pResort->workerArr);
                return 0;
            }
        }
    }
    


    // Allocate memory for arrays
    pResort->allClients = (Person**)malloc(pResort->clientCount * sizeof(Person*));
    if (!pResort->allClients) {
        printf("Memory allocation failed for allClients\n");
        return 0;
    }

    if (pResort->clientCount > 0)
    {
        for (int i = 0; i < pResort->clientCount; i++) {
            pResort->allClients[i] = (Person*)malloc(sizeof(Person));
            if (!pResort->allClients[i]) {
                printf("Memory allocation failed for allClients[%d]\n", i);
                // Clean up previously allocated memory
                for (int j = 0; j < i; j++)
                    free(pResort->allClients[j]);
                free(pResort->allClients);
                return 0;
            }
            int tempID;
            if (!readIntFromFile(&tempID, fp, "Error read client ID from binary file\n"))
                return 0;
            pResort->allClients[i] = findPerson(pResort->allPeople, tempID, allPeopleCount);
            if (pResort->allClients[i] == NULL)
            {
                printf("Error , there is no people in resort with this ID: %d", tempID);
                freePerson(pResort->allClients[i]);
                return 0;
            }
        }
    }



    pResort->allReservations = (Reservation*)malloc(pResort->reservationCount * sizeof(Reservation));
    if (!pResort->allReservations) {
        printf("Memory allocation failed for allReservations\n");
        free(pResort->allClients); // Clean up previously allocated memory
        free(pResort->workerArr);
        return 0;
    }


    // Load allReservations
    if (pResort->reservationCount > 0)
    {
        for (int i = 0; i < pResort->reservationCount; i++) {
            if (!loadReservationFromBinaryFile(&pResort->allReservations[i], pResort->allClients, pResort->clientCount, fp))
            {
                printf("Error loading reservation %d from file\n", i + 1);
                for (int j = i; j < pResort->reservationCount; j++)
                {
                    freeReservation(&pResort->allReservations[j]);
                }
                pResort->reservationCount = pResort->reservationCount - (pResort->reservationCount - i);
                return 0;
            }
        }
    }


    // Load theMap
    pResort->theMap = (SkiMap*)malloc(sizeof(SkiMap));
    if (!pResort->theMap) {
        printf("Memory allocation failed for theMap\n");
        return 0;
    }
    if (!loadSkiMapFromBinaryFile(pResort->theMap, pResort->workerArr, pResort->workerCount, fp)) {
        printf("Error loading SkiMap from file\n");
        free(pResort->theMap);
        return 0;
    }
    

    return 1; // Success
}


