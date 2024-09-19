#ifndef __INFO__
#define __INFO__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Date.h"
#include "General.h"


typedef enum { eSnowBoard, eSki, eSlide, eNofSkiingTypes } eSkiType;
static const char* skiingTypeStr[eNofSkiingTypes] = { "SnowBoard", "Ski", "Slide" };

typedef enum { eOneVali, eTwoVali, eThreeVali, eNofSkiPassTypes } eSkiPassType;
static const char* skiPassTypeStr[eNofSkiPassTypes] = { "OneVali", "TwoVali", "ThreeVali" };

typedef struct
{
	Date startDate;
	Date endDate;
	eSkiPassType skiPass;
	eSkiType skiingType;
} Info;

void         initInfo(Info* pInfo);
eSkiType     getSkiType();
eSkiPassType getSkiPassType();
Date         getStartVacationDate(Info* pInfo);
Date         getEndVacationDate(Info* pInfo);
char*        getDatesOfVacation(const Info* pInfo);
void         printInfo(const Info* pInfo);

int          saveInfoToFile(const Info* pInfo, FILE* fp);
int          loadInfoFromFile(Info* pInfo, FILE* fp);
int          saveCompressionInfo(const Info* pInfo, FILE* fp);
int          loadCompressionInfo(Info* pInfo, FILE* fp);

#endif