#include "Bytes.hpp"
#include "stdlib.h"
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

int Bytes::bytesJoin(const char *, const char *, int len) {

}