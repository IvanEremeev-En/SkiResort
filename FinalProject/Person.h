#ifndef __PERSON__
#define __PERSON__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "General.h"
#include "fileHelper.h"

#define MIN_AGE 0
#define MAX_AGE 100
#define ID_LEN 9
#define MIN_ID 100000000
#define MAX_ID 999999999


typedef enum { eMan, eWoman, eBoy, eGirl, eNofTypes } Type;
static const char* typeTitle[eNofTypes] = { "Man", "Woman", "Boy","Girl" };

typedef struct
{
	char*	name;
	int id;
	int age;
	Type  theType;
} Person;


void     initPerson(Person* pPer,Person* personArr,int personCount);
int      getPersonId(Person* planeArr, int planeCount);
int      getPersonName(Person* pPer);
int	     isIDUnique(Person* personArr, int personCount, int num);
int      checkAge(int age);
Type     getTypeFromUser();
void     printPerson(const Person* pPer);
void     freePerson(void* pPer);
Person*  findPerson(Person** personArr, int id, int personCount);

int	     savePersonToBinaryFile(const Person* pPer, FILE* fp); 
int	     loadPersonFromBinaryFile(Person* pPer, FILE* fp); 

int	     savePersonToFile(const Person* pPer, FILE* fp);
int	     loadPersonFromFile(Person* pPer, FILE* fp);
#endif