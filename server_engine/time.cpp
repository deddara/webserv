//#include "server.hpp"
#include <sys/time.h>
#include "iostream"
#include "time.h"


#define FEB_NF_MONTH 28
#define FEB_F_MONTH 29
#define F_MONTH 31
#define MONTH 30
# define SEC_PER_D 86400

# define SEC_PER_Y 31536000
# define SEC_PER_V_Y 31622400

int get_year(time_t & sec){

	int count = 0;
	int y = 0;
	int dec = SEC_PER_Y;
	while (sec - dec >= 0)
	{
		if (y == 3)
		{
			dec = SEC_PER_V_Y;
			y = 0;
		}
		else if (!y)
			dec = SEC_PER_Y;
		sec -= dec;
		count++;
		y++;
	}
	count += 70;
	return count;
}

//int get_month(time_t sec){
//
//}

//int get_day_of_month(time_t sec){
//	int day = 1;
//	int month = 1;
//	int year;
//	while (sec >= 0)
//	{
//		if (day == )
//	}
//}

struct tm * my_localtime(time_t & sec, 	struct tm & t)
{
	t.tm_year = get_year(sec);
//	t->tm_month = get_month(sec);
	return (&t);
}

int main()
{
	struct timeval tv;
	time_t t;
	struct tm *info;
	struct tm dmt;

	gettimeofday(&tv, NULL);
	t = tv.tv_sec;

	info = my_localtime(t, dmt);
	std::cout << info->tm_year;
}