//#include "server.hpp"
#include <sys/time.h>
#include "time.h"
#include "includes.hpp"

# define SEC_PER_Y 31536000
# define SEC_PER_V_Y 31622400

static int get_year(time_t & sec, int & is_v_y, int & day){

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
	}
	count += 70;
	return (count);
}

# define THIRTY_ONE_M	2678400
# define THIRTY_M		2592000
# define TWENTY_NINE_M	2505600
# define TWENTY_EIGHT_M	2419200


static int get_month(time_t & sec, int const & is_v_y){
	int dec = THIRTY_ONE_M;
	int month = 0;
	while (sec - dec >= 0)
	{
		if (month == 0 || month == 2 || month == 4 || month == 6 || \
		month == 7 || month == 9 || month == 11)
			dec = THIRTY_ONE_M;
		else if (month == 1 && is_v_y)
			dec = TWENTY_NINE_M;
		else if (month == 1 && !is_v_y)
			dec = TWENTY_EIGHT_M;
		else
			dec = THIRTY_M;
		sec -= dec;
		month++;
		if (month == 12)
			month = 0;
	}
	return month;
}

static int get_day(time_t & sec, int & day, int const & month, int const & is_v_y)
{
	int day_count = 1;
	while (sec - 86400 >= 0)
	{
		if ((month == 0 || month == 2 || month == 4 || month == 6 || \
		month == 7 || month == 9 || month == 11) && day_count == 31)
			day_count = 1;
		else if (month == 1 && day_count == 29 && is_v_y)
			day_count = 1;
		else if (month == 1 && day_count == 28 && !is_v_y)
			day_count = 1;
		else if ((month == 3 || month == 5 || month == 8 || month == 10) && day_count == 30)
			day_count = 1;
		day_count++;
		if (day == 7)
			day = 0;
		sec -= 86400;
	}
	return day_count;
}

static int get_hour(time_t & sec){

	int dec = 3600;
	int hour = 0;

	while (sec - dec >= 0)
	{
		hour++;
		sec -= dec;
	}

	return hour;
}

static int get_min(time_t & sec) {
	int dec = 60;
	int min = 0;
	while (sec - dec >= 0)
	{
		min++;
		sec -= dec;
	}
	return min;
}

static void get_day_of_week(time_t  sec, int & day){
	while (sec - 86400 >= 0)
	{
		day++;
		if (day == 7)
			day = 0;
		sec -= 86400;
	}
}

static std::string date_prepare(time_t & sec, struct tm & t)
{
	char buffer[40];
	int is_v_year;
	int day;

	bzero(buffer, sizeof(buffer));
	t.tm_year = get_year(sec, is_v_year, day);
	get_day_of_week(sec, day);
	t.tm_mon = get_month(sec, is_v_year);
	t.tm_mday = get_day(sec, day, t.tm_mon, is_v_year);
	t.tm_wday = day;
	t.tm_hour = get_hour(sec);
	t.tm_min = get_min(sec);
	t.tm_sec = sec;

	strftime (buffer, sizeof buffer, "%a, %d %b %G %k:%M:%S GMT\r\n", &t);
	std::string date(buffer);
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
