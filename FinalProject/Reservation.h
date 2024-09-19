#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Info.h"
#include "Person.h"



#define ONE_VALI_PRICE 100
#define TWO_VALI_PRICE 150
#define THREE_VALI_PRICE 200
#define CURRENCY '$'

typedef struct
{
	Info reservationInfo;
	Person* client;
	
} Reservation;

int     initReservation(Reservation* pReserv , Person* pPer);
char*   getDateOfReservation(const Reservation* pReserv);
int     getCostOfSkiPass(const Reservation* pReserv);
void    printReservation(Reservation* pReserv);

int	    saveReservationToFile(const Reservation* pReserv, FILE* fp); 
int	    loadReservationFromFile(Reservation* pReservation,Person** personArr,int personCount, FILE* fp); 

int	    saveReservationToBinaryFile(const Reservation* pRest, FILE* fp);
int	    loadReservationFromBinaryFile(Reservation* pRest, Person** personArr, int personCount, FILE* fp);

int		compareReservationByStartDate(const void* reservation1, const void* reservation2);
int		compareReservationBySkiPass(const void* reservation1, const void* reservation2);
int		compareReservationBySkyType(const void* reservation1, const void* reservation2);

void    freeReservation(Reservation* pReservation);