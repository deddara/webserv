#include "./server_engine/Server.hpp"
#include "includes.hpp"

int main()
{
	Server	serv;

	if (serv.setup() == 500)
		return 1;
	if (serv.launch())
		return 1;
}