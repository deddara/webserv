#ifndef BYTES_H
# define BYTES_H

class Bytes{
private:
	int bytes_count;

public:
	Bytes();

	const int & getBytes();
	void setBytes(const int &);
	void bytesCount(const int &);

	char* bytesDup(char*, char*,int len);
	char * bytesJoin(char*, char*,int len);
};

#endif