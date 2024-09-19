#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '-'

void getCorrectDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("Enter Date dd%c%cmm%c%cyyyy  minimum year %d\t",
			SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, MIN_YEAR);
		myGets(date, MAX_STR_LEN);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Date Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 12)
		return 0;
	if ((date[2] != SPECIAL_TAV) || (date[3] != SPECIAL_TAV)
		|| (date[6] != SPECIAL_TAV) || (date[7] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%*c%d%*c%*c%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

int		compareDate(const void* d1, const void* d2)
{
	const Date* pDate1 = (const Date*)d1;
	const Date* pDate2 = (const Date*)d2;
	if (pDate1->year > pDate2->year)
		return 1;
	if (pDate1->year < pDate2->year)
		return -1;

	if (pDate1->month > pDate2->month)
		return 1;

	if (pDate1->month < pDate2->month)
		return -1;

	if (pDate1->day > pDate2->day)
		return 1;

	if (pDate1->day < pDate2->day)
		return -1;

	return 0;
}


int    saveDateToTextFile(const Date* pDate, FILE* fp)
{
	if (pDate == NULL || fp == NULL)
		return 0;
	fprintf(fp, "%d\n", pDate->day);
	fprintf(fp, "%d\n", pDate->month);
	fprintf(fp, "%d\n", pDate->year);
	return 1;
}

int    loadDateFromTextFile(Date* pDate, FILE* fp)
{
	if (pDate == NULL || fp == NULL)
		return 0;
	fscanf(fp, "%d", &pDate->day);
	fgetc(fp);
	fscanf(fp, "%d", &pDate->month);
	fgetc(fp);
	fscanf(fp, "%d", &pDate->year);
	fgetc(fp);
	return 1;
}

int     saveCompressionDate(FILE* fp, const Date* pDate)
{
	BYTE data[3];
	data[0] = pDate->year >> 7;
	data[1] = (pDate->month >> 3) | (pDate->year << 1);
	data[2] = pDate->day | (pDate->month << 5);


	if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
	{
		return 0;
	}
	return 1;
}

int     loadCompressionDate(FILE* fp, Date* pDate)
{
	BYTE data[3];
	if (fread(data, sizeof(char), 3, fp) != 3)
		return 0;

	pDate->year = (data[1] >> 1) | ((data[0] & 0xF) << 7);
	pDate->month = (data[2] >> 5) | ((data[1] & 0x1) << 3);
	pDate->day = data[2] & 0x1F;

	return 1;
}

char* getOnlyDate(const Date* pDate)
{
	char* dateOnly = (char*)malloc(12 * sizeof(char)); // Allocate memory for the date string

	if (dateOnly == NULL) {
		// Memory allocation failed
		return NULL;
	}
	// Format the date string using sprintf
	sprintf(dateOnly, "%d/%d/%d", pDate->day, pDate->month, pDate->year);

	return dateOnly;
}

void printDate(const Date* pDate)
{
	printf("Date: %d/%d/%d\n", pDate->day, pDate->month, pDate->year);
}
