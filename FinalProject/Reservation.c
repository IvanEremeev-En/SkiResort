#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reservation.h"




int initReservation(Reservation* pReserv , Person* pPer)
{
	pReserv->client = pPer;
	initInfo(&pReserv->reservationInfo);
	if (&pReserv->reservationInfo == NULL)
		return 0;
	return 1;
}


char* getDateOfReservation(const Reservation* pReserv)
{
	return getDatesOfVacation(&pReserv->reservationInfo);

}



int getCostOfSkiPass(const Reservation* pReserv)
{
	Info reservInfo = pReserv->reservationInfo;
	if (reservInfo.skiPass == 0)
		return ONE_VALI_PRICE;
	if (reservInfo.skiPass == 1)
		return TWO_VALI_PRICE;
	else
		return THREE_VALI_PRICE;

}

int		compareReservationByStartDate(const void* reservation1, const void* reservation2)
{
	const Reservation* pReservation1 = (const Reservation*)reservation1;
	const Reservation* pReservation2 = (const Reservation*)reservation2;


	return compareDate(&pReservation1->reservationInfo.startDate, &pReservation2->reservationInfo.startDate);

}
int		compareReservationBySkiPass(const void* reservation1, const void* reservation2)
{
	const Reservation* pReservation1 = (const Reservation*)reservation1;
	const Reservation* pReservation2 = (const Reservation*)reservation2;

	return pReservation1->reservationInfo.skiPass - pReservation2->reservationInfo.skiPass;

}
int		compareReservationBySkyType(const void* reservation1, const void* reservation2)
{
	const Reservation* pReservation1 = (const Reservation*)reservation1;
	const Reservation* pReservation2 = (const Reservation*)reservation2;

	return pReservation1->reservationInfo.skiingType - pReservation2->reservationInfo.skiingType;

}

int saveReservationToFile(const Reservation* pReserv, FILE* fp) {
	if (pReserv == NULL || fp == NULL) {
		return 0;
	}
	fprintf(fp, "%d\n", pReserv->client->id);
	if (!saveInfoToFile(&pReserv->reservationInfo, fp))
		return 0;
	
	return 1;
}

int loadReservationFromFile(Reservation* pReservation, Person** personArr, int personCount, FILE* fp) 
{
	if (pReservation == NULL || fp == NULL || personArr == NULL ) {
		return 0;
	}
	pReservation->client = (Person*)malloc(sizeof(Person));
	if (!pReservation->client)
	{
		printf("Error: Memory allocation failed\n");
		return 0;
	}
	int tempPersonID;
	fscanf(fp, "%d", &tempPersonID);
	fgetc(fp);
	pReservation->client = findPerson(personArr, tempPersonID, personCount);
	if (pReservation->client == NULL)
	{
		freePerson(pReservation->client);
		return 0;
	}

	if (!loadInfoFromFile(&pReservation->reservationInfo, fp))
		return 0;
	return 1;
}

int saveReservationToBinaryFile(const Reservation* pReserv, FILE* fp) 
{
	if (pReserv == NULL || fp == NULL) {
		return 0;
	}
	if (!writeIntToFile(pReserv->client->id, fp, "Error write ID of client\n"))
		return 0;
	if (!saveCompressionInfo(&pReserv->reservationInfo, fp))
		return 0;
	return 1;
}

int loadReservationFromBinaryFile(Reservation* pReserv, Person** personArr, int clientCount, FILE* fp) 
{
	if (pReserv == NULL || fp == NULL || personArr == NULL) {
		return 0;
	}

	int tempID;
	if (!readIntFromFile(&tempID, fp, "Error read client ID from binary file\n"))
		return 0;
	// Load client information
	pReserv->client = (Person*)malloc(sizeof(Person));
	if (!pReserv->client) {
		printf("Error: Memory allocation failed\n");
		return 0;
	}
	pReserv->client = findPerson(personArr, tempID, clientCount);
	if (pReserv->client == NULL) {
		freeReservation(pReserv);
		printf("Error find person with %d ID in our resort\n", tempID);
		return 0;
	}

	if (!loadCompressionInfo(&pReserv->reservationInfo, fp))
		return 0;

	return 1;
}

void freeReservation(Reservation* pReservation)
{
	if (pReservation == NULL) {
		return; // Do nothing if person is NULL
	}
	freePerson(pReservation->client);
	free(pReservation);
}

void printReservation(Reservation* pReserv)
{
	printPerson(pReserv->client);
	printf(" Your Reservation Info\n ");
	printInfo(&pReserv->reservationInfo);
}
