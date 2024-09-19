#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "person.h"
#include "General.h"



void	initPerson(Person* pPer, Person* personArr, int personCount)
{
	pPer->id = getPersonId(personArr, personCount);
	getPersonName(pPer);
	printf("What is your age?\n");
	int tempAge; 
	scanf("%d",&tempAge);
	if (checkAge(tempAge))
		pPer->age = tempAge;
	else
		printf("Sorry but our Ski-Resort is adjusted to age 1-100\n");
	pPer->theType = getTypeFromUser();
	getchar();
}

int getPersonName(Person* pPer)
{
	char temp[255];
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;

	while (count == 0)
	{
		printf("Enter Person name please\n");
		myGets(temp, MAX_STR_LEN);
		wordsArray = splitCharsToWords(temp, &count, &totalLength);
	}
	pPer->name = allocateRightLength(wordsArray, count, totalLength);
	if (!pPer->name)
		return 0;

	if (count == 1)
		changeNameOneWord(pPer->name, wordsArray[0]);
	else
		changeName(pPer->name, count, wordsArray);

	//clean temp data
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);
	return 1;
}

Person* findPerson(Person** personArr, int id, int personCount)
{
	if (personArr == NULL) {
		return NULL;  // 
	}

	for (int i = 0; i < personCount; i++) {
		if (personArr[i]->id == id) {
			return personArr[i];  
		}
	}

	return NULL;  
}


int checkAge(int age)
{
	if (age<MIN_AGE || age>MAX_AGE)
		return 0;
	return 1;

}


int getPersonId(Person* personArr, int personCount)
{
	int num;
	int unique = 0;
	do {
		printf("Enter person ID - it's ha to be %d digits : \n", ID_LEN);
		scanf("%d", &num);
		if (num >= MIN_ID && num <= MAX_ID)
			unique = isIDUnique(personArr, personCount, num);
	} while (!unique);
	return num;
}

int	isIDUnique(Person* personArr, int personCount, int num)
{
	for (int i = 0; i < personCount; i++)
	{
		if (personArr[i].id == num)
			return 0;
	}
	return 1;
}

Type	getTypeFromUser()
{
	int i, t;
	printf("Enter person type\n");
	do {
		for (i = 0; i < eNofTypes; i++)
			printf("Enter %d for %s\n", i, typeTitle[i]);
		scanf("%d", &t);
	} while (t < 0 || t >= eNofTypes);
	return (Type)t;
}

void	printPerson(const Person* pPer)
{
	printf("%s name %s , id %d , age %d\n", typeTitle[pPer->theType], pPer->name, pPer->id , pPer->age);
}

int	  savePersonToBinaryFile(const Person* pPer, FILE* fp)
{
	if (fp == NULL || pPer == NULL) {
		printf("Error: File pointer is NULL or there is no Person\n");
		return 0;
	}
	if (!writeStringToFile(pPer->name, fp, "Error write person name to binary file!\n"))
		return 0; 
	if (!writeIntToFile(pPer->id, fp, "Error write person ID to binary file\n"))
		return 0;
	if (!writeIntToFile(pPer->age, fp, "Error write person age to binary file\n"))
		return 0;
	if (!writeIntToFile(pPer->theType, fp, "Error write person gender to binary file\n"))
		return 0;

	return 1;
}

int	  loadPersonFromBinaryFile(Person* pPer, FILE* fp)
{
	if (fp == NULL || pPer == NULL) {
		printf("Error: File pointer is NULL or there is no Person\n");
		return 0;
	}
	pPer->name = readStringFromFile(fp, "Error read person name\n");
	if (!pPer->name)
		return 0;
	if (!readIntFromFile(&pPer->id, fp, "Error read person ID from binary file\n"))
	{
		freePerson(pPer);
        return 0;
	}
	if (!readIntFromFile(&pPer->age, fp, "Error read person age from binary file\n"))
	{
		freePerson(pPer);
		return 0;
	}
	int gender;
	if (!readIntFromFile(&gender, fp, "Error read person gender from binary file\n"))
	{
		freePerson(pPer);
		return 0;
	}
	pPer->theType = gender;
	return 1;

}

int	  savePersonToFile(const Person* pPer, FILE* fp)
{
	if (fp == NULL || pPer == NULL) {
		printf("Error: File pointer is NULL or there is no Person\n");
		return 0;
	}
	fprintf(fp, "%s\n", pPer->name);
	fprintf(fp, "%d\n", pPer->id);
	fprintf(fp, "%d\n", pPer->age);
	fprintf(fp, "%d\n", pPer->theType);
	
	return 1;

}

int	  loadPersonFromFile(Person* pPer, FILE* fp)
{
	if (fp == NULL || pPer == NULL) {
		printf("Error: File pointer is NULL or there is no Person\n");
		return 0;
	}
	char tempName[MAX_STR_LEN];
	myGetsF(tempName, MAX_STR_LEN, fp);
	pPer->name = getDynStr(tempName);

	fscanf(fp, "%d", &pPer->id);
	fgetc(fp);
	fscanf(fp, "%d", &pPer->age);
	fgetc(fp);
	fscanf(fp, "%d", &pPer->theType);
	fgetc(fp);
	return 1;
}


void	freePerson(void* pPer)
{
	Person* tPer = *(Person**)pPer;
	free(tPer->name);
	// Finally, free the Person object itself
	free(pPer);
}