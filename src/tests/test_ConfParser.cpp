#include "ConfParser.hpp"

int					main(int argc, char **argv)
{
	ConfParser *	cnfPrs = nullptr;

	if (argc > 2)
	{
		std::cout << "Error: too much arguments" << std::endl;
		return 1;
	}
	else if (argc == 2)
		cnfPrs = new ConfParser(argv[1]);
	else
		cnfPrs = new ConfParser();
	std::cout << cnfPrs->getPath() << std::endl;
	return 0;
}
