#ifndef __WORKER__
#define __WORKER__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Person.h"




#define MIN_SN 1
#define MAX_SN 9999

typedef enum { eGondolaOperator, eInstructor, eDirector, eNofJobTypes } eJobType;
static const char* jobTypeStr[eNofJobTypes] = { "GondolaOperator", "Instructor", "Director" };

typedef struct
{
	Person* worker;
	int serialNum;
	double salary;
	eJobType jobType;

} Worker;

int          initWorker(Worker* pWorker,Person* pPer, Worker* workerArr , int workerCount);
eJobType     getJobTypeOfWorker();
const char*  GetWorkerJobTypeStr(int type);
int          getWorkerSN(Worker* workerArr, int workerCount);
int		     isSerialNumUnique(Worker* workerArr, int workerCount, int num);
Worker*      findWorkerBySN(Worker* workerArr, int count, int sn);
double       getWorkerSalary(Worker* pWorker);
void         printWorker(const Worker* pWorker);
void         freeWorker(Worker* pWorker); 

int		     loadWorkerFromFile(Worker* pWork, Person** personArr, int personCount, FILE* fp);
int		     saveWorkerToFile(const Worker* pWork, FILE* fp); 

int	         saveWorkerToBinaryFile(const Worker* pWork, FILE* fp);
int	         loadWorkerFromBinaryFile(Worker* pWork, Person** personArr, int personCount, FILE* fp);

#endif