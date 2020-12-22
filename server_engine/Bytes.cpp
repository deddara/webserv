#include "Bytes.hpp"
#include "stdlib.h"
#include <iostream>
Bytes::Bytes() : bytes_count(0) {}

const int & Bytes::getBytes() { return bytes_count; }

void Bytes::setBytes(const int &num) { bytes_count = num;}
void Bytes::bytesCount(const int &num){
	bytes_count += num;
}

char * Bytes::bytesDup(char *dst, char *src, int len) {
	if (!(dst = (char*)malloc(sizeof(char) * len + 1)))
		return NULL;
	for (int i = 0; i < len; ++i)
	{
		*dst = *src;
		dst++;
		src++;
	}
	dst[len] = '\0';
	return (dst - len);
}

char *Bytes::bytesJoin(char *dst, char *src, int len) {
	char *res;

	if (!(res = (char*)malloc(sizeof(char) * (bytes_count + len + 1))))
		return NULL;

	for (int i = 0; i < bytes_count; ++i)
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
	std::cout << res - bytes_count - len;
	return res - bytes_count - len;
}