#ifndef BYTES_H
# define BYTES_H

class Bytes{
private:
	unsigned long bytes_count;

public:
	Bytes();

	const unsigned long & getBytes() const;
	void setBytes(const unsigned long &);
	void bytesCount(const int &);

	char* bytesDup(char*, char*,int const & len);
	char * bytesJoin(char*, char*,int const & len);
	char * bytesCut(char*, unsigned long const &);
};

#endif