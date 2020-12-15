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

int get_year(time_t & sec, int & is_v_y, int & day){

	int count = 0;
	int y = 1; //because 1972 closest lap year
	int dec = SEC_PER_Y;
	day = 4; //because in 1970 1 Jan was Thursday
	while (sec - dec >= 0)
	{
		if (day == 7)
			day = 0;
		else if (day >= 7)
			day = 1;
		if (y == 3)
		{
			dec = SEC_PER_V_Y;
			day++;
			y = -1;
		}
		else if (!y)
			dec = SEC_PER_Y;
		day++;
		sec -= dec;
		count++;
		y++;
	}
	if (y == 3) {
		is_v_y = 1;
		std::cout << day << std::endl;
	}
	count += 70;
	return (count);
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
	int is_v_year;
	int day;
	t.tm_year = get_year(sec, is_v_year, day);
//	t->tm_month = get_month(sec);
	return (&t);
}

int main()
{
	struct timeval time_of_day_res;
	time_t		time_in_sec;
	struct tm *info;
	struct tm dmt;

	gettimeofday(&time_of_day_res, NULL);
	time_in_sec = time_of_day_res.tv_sec;

	info = my_localtime(time_in_sec, dmt);

	std::cout << time_in_sec << std::endl;

	std::cout << info->tm_year;
}