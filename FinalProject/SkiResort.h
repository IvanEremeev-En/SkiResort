#ifndef __SKI_SERORT__
#define __SKI_RESORT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SkiMap.h"
#include "Reservation.h"

#define INITIAL_RESERVATION_CAPACITY 1
#define INITIAL_WORKER_CAPACITY 1
#define INITIAL_CLIENT_CAPACITY 1


typedef struct
{
	char* name;
	SkiMap* theMap;
	Reservation* allReservations;
	Worker* workerArr;
	Person** allClients;
	Person** allPeople;
	int reservationCount;
	int workerCount; 
	int clientCount;
} SkiResort;



int        initSkiResort(SkiResort* pSkiRes, char* name);  
int        initStation(SkiResort* pSkiRes); 
int        addReservation(SkiResort* pSkiRes); 
int        addWorker(SkiResort* pSkiRes);
int        addPerson(SkiResort* pSkiRes);
void       sortReservations(SkiResort* pSkiRes);
int        showSortMenu(); 
int        getProfitOfResort(const SkiResort* pSkiRes);
void       printWorkersArr(Worker* workersArr, int workerCount);
void       printReservationArr(Reservation* ReservationsArr, int reservationCount);
void       printClientArr(Person** clientsArr, int clientCount);
void       printSkiResort(const SkiResort* pSkiRes);

int		   saveSkiResortToFile(const SkiResort* pResort, FILE* fp);
int		   loadSkiResortFromFile(SkiResort* pResort, FILE* fp);

int	       saveSkiResortToBinaryFile(const SkiResort* pResort, FILE* fp);
int	       loadSkiResortFromBinaryFile(SkiResort* pResort, FILE* fp);

void       freeWorkerArr(Worker* WorkeAarr, int size);
void       freeClientArr(Person** clientArr, int size);
void       freeAllPeopleArr(Person** allPeople, int size);
void       freeSkiResort(SkiResort* pSkiRes);





#endif