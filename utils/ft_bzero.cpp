#include <stdlib.h>

void    ft_bzero(void *b, size_t n)
{
	unsigned char	*dest;
	size_t			i;

	dest = (unsigned char *)b;
	i = 0;
	while (i++ < n)
		*dest++ = 0;
}

