
#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2020

typedef unsigned char BYTE;

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
int		compareDate(const void* d1, const void* d2);
void	printDate(const Date* pDate);
int     saveDateToTextFile(const Date* pDate, FILE* fp);
int     loadDateFromTextFile(Date* pDate, FILE* fp);
int     saveCompressionDate(FILE* fp, const Date* pDate);
int     loadCompressionDate(FILE* fp, Date* pDate);

char*   getOnlyDate(const Date* pDate);


#endif 