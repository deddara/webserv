//#include "server.hpp"
#include <sys/time.h>
#include "time.h"
#include "server.hpp"


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

void get_day(time_t & sec, int & day)
{

	while (sec - 86400 >= 0)
	{
		day++;
		if (day == 7)
			day = 0;
		sec -= 86400;
	}
}

std::string date_prepare(time_t & sec, struct tm & t)
{
	char buffer[40];
	int is_v_year;
	int day;
	int i = 0;

	bzero(buffer, sizeof(buffer));
	t.tm_year = get_year(sec, is_v_year, day);
	get_day(sec, day);
	t.tm_wday = day;

	strftime (buffer, sizeof buffer, "%a, ", &t);
	int j = 4;
	for (i = 5; i < 16; ++i){
		if (j == 7)
			j = 0;
		else if (j == 4 && i != 5)
			j = 7;
		buffer[i] = __DATE__[j];
		j++;
	}
	buffer[i] = ' ';
	std::string date(buffer);
	date.append(__TIME__);
	date.append(" GMT\r\n");
	return date;
}

std::string my_localtime()
{
	struct timeval time_of_day_res;
	time_t		time_in_sec;
	struct tm dmt;
	std::string date;

	gettimeofday(&time_of_day_res, NULL);
	time_in_sec = time_of_day_res.tv_sec;

	date = date_prepare(time_in_sec, dmt);
	return (date);
}
