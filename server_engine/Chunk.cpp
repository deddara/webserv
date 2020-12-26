/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Chunk.cpp                                                  | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 22:50:02 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 19:09:40 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Chunk.hpp"
#include "includes.hpp"
#include "string"

const int & Chunk::getCount(void){ return count; }
const int & Chunk::getLenSum(void) { return len_sum; }
const int & Chunk::getLen(void) { return len; }
const int & Chunk::getHexLen(void) { return hex_len; }
const int & Chunk::getBuffSum() { return buff_sum; }

void	Chunk::setLenSum(const int &num) { len_sum = num; }
void	Chunk::setLen(const int &num) { len = num; }
void	Chunk::setCount(const int &num) { count = num; }
void Chunk::setBuffSum(const int &num) { buff_sum = num; }

int		Chunk::takeNum(const char * src,const int &ln){

	if (!ft_strnstr(src, "\r\n", ln))
		return (1);
	std::string str_num;
	char *p;
	while (*src != '\r' && *(src + 1) != '\n')
	{
		str_num.push_back(*src);
		src++;
	}

	long n = strtol( str_num.c_str(), &p, 16);
	if (*p != 0)
		return (-1);
	else {
		hex_len = str_num.length() + 2;
		len = n;
	}
	return (0);
}

void Chunk::setZero() {
	count = 0;
	len_sum = 0;
	len = 0;
	hex_len = 0;
}
