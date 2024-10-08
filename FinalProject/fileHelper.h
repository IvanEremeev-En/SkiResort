#pragma once

int		writeStringToFile(const char* str, FILE* fp, const char* msg);
int		writeCharsToFile(const char* arr, int size, FILE* fp, const char* msg);
int		writeIntToFile(int val, FILE* fp, const char* msg);
int	 writeDoubleToFile(double val, FILE* fp, const char* msg);

char*  readStringFromFile(FILE* fp, const char* msg);
int		readFixSizeStrFromFile(char* arr, FILE* fp, const char* msg);
int		readCharsFromFile(char* arr, int size, FILE* fp, const char* msg);
int		readIntFromFile(int* val, FILE* fp, const char* msg);
int	    readDoubleFromFile(double* pVal, FILE* fp, const char* msg);