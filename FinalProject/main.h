

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SkiResort.h"

#define REGULAR_FILE_NAME "skiResortFile.txt"
#define BINARY_FILE_NAME "skiResortBinaryFile.bin"


void handleCustomerMenu(SkiResort* resort);
void handleManagerMenu(SkiResort* resort);


int main();

#endif // _MAIN_H_