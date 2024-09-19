#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Worker.h"

int initWorker(Worker* pWorker,Person* pPer, Worker* workerArr, int workerCount)
{
	if (pWorker == NULL || pPer == NULL || workerArr == NULL || workerCount < 0) {
		return 0; // Error: Invalid input
	}
	pWorker->worker = pPer;
	pWorker->serialNum = getWorkerSN(workerArr, workerCount);
	pWorker->jobType = getJobTypeOfWorker();
	pWorker->salary = getWorkerSalary(pWorker);

	return 1;
}

int getWorkerSN(Worker* workerArr, int workerCount)
{
	int num;
	int unique = 0;
	do {
		printf("Enter this worker serial number - between %d to %d\n", MIN_SN, MAX_SN);
		scanf("%d", &num);
		if (num >= MIN_SN && num <= MAX_SN)
			unique = isSerialNumUnique(workerArr, workerCount, num);
	} while (!unique);
	return num;
}

int		isSerialNumUnique(Worker* workerArr, int workerCount, int num)
{
	for (int i = 0; i < workerCount; i++)
	{
		if (workerArr[i].serialNum == num)
			return 0;
	}
	return 1;
}

Worker* findWorkerBySN(Worker* workerArr, int count, int sn)
{
	for (int i = 0; i < count; i++)
		if (workerArr[i].serialNum == sn)
			return &workerArr[i];
	return NULL;
}


double getWorkerSalary(Worker* pWorker)
{
	double tempSalary;
	do
	{
		printf("Please enter %s worker salary per mount ->\n", pWorker->worker->name);
		scanf("%lf", &tempSalary);
		if (tempSalary > 0)
			break;
	} while (1);
	return tempSalary;
}


eJobType getJobTypeOfWorker()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofJobTypes; i++)
			printf("%d for %s\n", i, jobTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofJobTypes);
	getchar();
	return (eJobType)option;

}

int		saveWorkerToFile(const Worker* pWork, FILE* fp)
{
	if (fp == NULL || pWork ==NULL) {
		printf("Error: File pointer is NULL or worker\n");
		return 0;
	}

	// save worker person ID
	fprintf(fp, "%d\n", pWork->worker->id);

	// Save Worker data
	fprintf(fp, "%d\n", pWork->serialNum);
	fprintf(fp, "%f\n", pWork->salary);
	fprintf(fp, "%d\n", pWork->jobType);
	return 1;

}

int		loadWorkerFromFile(Worker* pWork,Person** personArr, int personCount, FILE* fp)
{
	if (fp == NULL || pWork == NULL) {
		printf("Error: File pointer is NULL or worker\n");
		return 0;
	}

	// Allocate memory for worker
	pWork->worker = (Person*)malloc(sizeof(Person));
	if (pWork->worker == NULL) {
		printf("Error: Memory allocation failed\n");
		return 0;
	}

    // Load Person data
	int tempID;
	fscanf(fp, "%d", &tempID);
	fgetc(fp);
	pWork->worker = findPerson(personArr, tempID, personCount);
	if (pWork->worker == NULL)
	{
		free(pWork->worker);
		return 0;
	}

	// Load Worker data
	fscanf(fp, "%d", &pWork->serialNum);
	fgetc(fp);
	fscanf(fp, "%lf", &pWork->salary);
	fgetc(fp);
	fscanf(fp, "%d", &pWork->jobType);
	fgetc(fp);
	
	return 1;
}

int	  saveWorkerToBinaryFile(const Worker* pWork, FILE* fp)
{
	if (fp == NULL || pWork == NULL) {
		printf("Error: File pointer is NULL or worker\n");
		return 0;
	}
	
	if (!writeIntToFile(pWork->worker->id, fp, "Error write worker Id to binary file\n"))
		return 0;
	if (!writeIntToFile(pWork->serialNum, fp, "Error write worker SN to binary file\n"))
		return 0;
	if (!writeDoubleToFile(pWork->salary, fp, "Error write worker salary to binary file\n"))
		return 0;
	if (!writeIntToFile(pWork->jobType, fp, "Error write worker job type to binary file\n"))
		return 0;
	return 1;
}

int	  loadWorkerFromBinaryFile(Worker* pWork, Person** personArr, int personCount, FILE* fp)
{
	if (fp == NULL || pWork == NULL) {
		printf("Error: File pointer is NULL or worker\n");
		return 0;
	}

	// Allocate memory for Person
	pWork->worker = (Person*)malloc(sizeof(Person));
	if (pWork->worker == NULL) {
		printf("Error: Memory allocation failed\n");
		return 0;
	}

	// Load Person data
	int tempID;
	if (!readIntFromFile(&tempID, fp, "Error read worker ID from binary file\n"))
		return 0;
	pWork->worker = findPerson(personArr, tempID, personCount);
	if (pWork->worker == NULL)
	{
		free(pWork->worker);
		return 0;
	}
	
	if (!readIntFromFile(&pWork->serialNum, fp, "Error read worker SN from binary file\n"))
	{
		freePerson(pWork->worker);
		return 0;
	}
	if (!readDoubleFromFile(&pWork->salary, fp, "Error read worker salary from binary file\n"))
	{
		freePerson(pWork->worker);
		return 0;
	}
	int jobT;
	if (!readIntFromFile(&jobT, fp, "Error read worker job type from binary file\n"))
	{
		freePerson(pWork->worker);
		return 0;
	}
	pWork->jobType = jobT;

	return 1;
}



void         freeWorker(Worker* pWorker)
{
	if (pWorker == NULL) {
		return; // Do nothing if person is NULL
	}
	freePerson(pWorker->worker);
	free(pWorker);
}


const char* GetWorkerJobTypeStr(int type)
{
	if (type < 0 || type >= eNofJobTypes)
		return NULL;
	return jobTypeStr[type];
}



void printWorker(const Worker* pWorker)
{
	if (pWorker == NULL)
		return;
	printf("Worker:");
	printPerson(pWorker->worker);
	printf("His Serial Number %d , job type %s  and salary %.2lf\n", pWorker->serialNum ,GetWorkerJobTypeStr(pWorker->jobType), pWorker->salary);

}
