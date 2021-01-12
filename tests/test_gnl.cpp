#include "get_next_line.hpp"
#include <stdlib.h>

int	main(int argc, char *argv[])
{
	int		fd_test_me;
	int		fd_user_output;
	char	*line;
	int		result;

	line = NULL;
	result = 1;
	if (argc != 2)
	{
		printf("failed to provide the file to be opened as arg\n");
		return (0);
	}
	if((fd_test_me = open(argv[1], O_RDONLY)) == -1)
	{
		printf("failed to open");
		printf("filedescriptor.\n");
		return (0);
	}
	while (result > 0)
	{
		result = get_next_line(fd_test_me, &line);
		if (result != -1)
		{
			printf(line, fd_user_output);
			if (result)
				printf("\n");
		}
		free(line);
	}
	close(fd_test_me);
	return (0);
}
