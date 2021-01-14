/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Bytes.cpp                                                  | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 23:30:46 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 19:21:35 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Bytes.hpp"
#include "stdlib.h"
#include <iostream>
Bytes::Bytes() : bytes_count(0) {}

const unsigned long & Bytes::getBytes() const { return bytes_count; }

void Bytes::setBytes(const unsigned long &num) { bytes_count = num;}

void Bytes::bytesCount(const int &num){
	bytes_count += num;
}


char * Bytes::bytesDup(char *dst, const char *src, int const & len) {
	if (!(dst = (char*)malloc(sizeof(char) * len + 1)))
		return NULL;
	for (int i = 0; i < len; ++i)
	{
		*dst = *src;
		dst++;
		src++;
	}
	*dst = '\0';
	return (dst - len);
}

char *Bytes::bytesJoin(char *dst, const char *src, int const & len, int const & src_len) {
	char *res;

	if (!(res = (char*)malloc(sizeof(char) * (src_len + len + 1))))
		return NULL;

	for (int i = 0; i < src_len; ++i)
	{
		*res = *dst;
		res++;
		dst++;
	}
	for (int i = 0; i < len; ++i)
	{
		*res = *src;
		res++;
		src++;
	}
	*res = '\0';
	return res - src_len - len;
}

char * Bytes::bytesCut(char *src, unsigned long const & len) {
	char *res;

	if (!(res = (char*)malloc(sizeof(char) * len + 1)))
		return NULL;

	for (unsigned long i = 0; i < len; ++i)
	{
		*res = *src;
		src++;
		res++;
	}
	*res = '\0';
	bytes_count = len;
	return res - len;
}