#include "sys/socket.h"
#include <iostream>
#include "sstream"
#include "fstream"
#include "vector"

struct lst_adress{
	const char *ip;
	int 		port;
};

void 	get_ip_from_file(lst_adress & adr)
{
	std::string			file_data;
	std::stringstream	buffer;
	std::ifstream		ifs("conf.d");
	std::vector<std::string>	vec;

	if (!ifs.is_open())
		return;
	buffer << ifs.rdbuf(); //reading whole file
	file_data = buffer.str();	//converting to string

	std::string delimiter = ":";
	size_t pos = 0;
	std::string token;
	while ((pos = file_data.find(delimiter)) != std::string::npos) {
		token = file_data.substr(0, pos);
		vec.push_back(token);
		file_data.erase(0, pos + delimiter.length());
	}
	vec.push_back(file_data);
	adr.ip = vec.front().c_str();
	adr.port = std::stoi(vec.back());
}

int main()
{
	lst_adress	adr;
	get_ip_from_file(adr);

}
