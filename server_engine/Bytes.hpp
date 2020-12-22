#ifndef BYTES_H
# define BYTES_H

class Bytes{
private:
	int bytes_count;

public:
	Bytes();

	const int & getBytes();
	void setBytes(const int &);
};

#endif