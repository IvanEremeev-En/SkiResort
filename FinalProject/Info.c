#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Info.h"


void initInfo(Info* pInfo)
{
	printf("for beginning of vacation\n");
	getCorrectDate(&pInfo->startDate);
	printf("\nFor end of vacation\n");
	getCorrectDate(&pInfo->endDate);
	pInfo->skiingType = getSkiType();
	pInfo->skiPass = getSkiPassType();
}

eSkiType getSkiType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter with skiing type you want:\n");
		for (int i = 0; i < eNofSkiingTypes; i++)
			printf("%d for %s\n", i, skiingTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofSkiingTypes);
	getchar();
	return (eSkiType)option;
}

eSkiPassType getSkiPassType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter with SkiPass do you want to buy:\n");
		for (int i = 0; i < eNofSkiPassTypes; i++)
			printf("%d for %s\n", i, skiPassTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofSkiPassTypes);
	getchar();
	return (eSkiPassType)option;
}

Date getStartVacationDate(Info* pInfo)
{
	return pInfo->startDate;
}

Date getEndVacationDate(Info* pInfo)
{
	return pInfo->endDate;
}


char* getDatesOfVacation(const Info* pInfo)
{
	return (char*)("%s - %s", getOnlyDate(&pInfo->startDate), getOnlyDate(&pInfo->endDate));
}

int          saveInfoToFile(const Info* pInfo, FILE* fp)
{
	if (pInfo == NULL || fp == NULL)
		return 0;
	if (!saveDateToTextFile(&pInfo->startDate, fp))
		return 0;
	if(!saveDateToTextFile(&pInfo->endDate, fp))
		return 0;
	fprintf(fp, "%d\n", pInfo->skiPass);
	fprintf(fp, "%d\n", pInfo->skiingType);

	return 1;
}

int          loadInfoFromFile(Info* pInfo, FILE* fp)
{
	if (pInfo == NULL || fp == NULL)
		return 0;
	if (!loadDateFromTextFile(&pInfo->startDate, fp))
		return 0;
	if (!loadDateFromTextFile(&pInfo->endDate, fp))
		return 0;
	fscanf(fp, "%d", &pInfo->skiPass);
	fgetc(fp);
	fscanf(fp, "%d", &pInfo->skiingType);
	fgetc(fp);
	return 1;
}

int          saveCompressionInfo(const Info* pInfo, FILE* fp)
{
	BYTE data[8]; // 5 bytes for startDate + 5 bytes for endDate + 3 bytes for Info additional fields

	// Compressing startDate
	if (!saveCompressionDate(fp, &(pInfo->startDate))) {
		return 0; // Error occurred while compressing startDate
	}

	// Compressing endDate
	if (!saveCompressionDate(fp, &(pInfo->endDate))) {
		return 0; // Error occurred while compressing endDate
	}

	// Storing additional fields directly in the data array
	data[0] = pInfo->skiPass;
	data[1] = pInfo->skiingType;

	// Writing compressed data to file
	if (fwrite(data, sizeof(BYTE), 3, fp) != 3) {
		return 0; // Error occurred while writing additional fields
	}

	return 1;
}

int          loadCompressionInfo(Info* pInfo, FILE* fp)
{
	BYTE data[8]; // 5 bytes for startDate + 5 bytes for endDate + 3 bytes for Info additional fields

	// Loading compressed startDate
	if (!loadCompressionDate(fp, &(pInfo->startDate))) {
		return 0; // Error occurred while loading startDate
	}

	// Loading compressed endDate
	if (!loadCompressionDate(fp, &(pInfo->endDate))) {
		return 0; // Error occurred while loading endDate
	}

	// Reading additional fields directly from the data array
	if (fread(data, sizeof(BYTE), 3, fp) != 3) {
		return 0; // Error occurred while reading additional fields
	}

	// Assigning additional fields to pInfo
	pInfo->skiPass = data[0];
	pInfo->skiingType = data[1];

	return 1;
}


void printInfo(const Info* pInfo)
{
	printf("Your vacation starts in, ");
	printDate(&pInfo->startDate);
	printf("Your vacation ends in, ");
	printDate(&pInfo->endDate);
	printf("You choose %s type of SkiPass, and you do %s\n", skiPassTypeStr[pInfo->skiPass], skiingTypeStr[pInfo->skiingType]);
}
