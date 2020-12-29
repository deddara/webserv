//
// Created by Janel Jeremiah on 12/28/20.
//

#include <stdlib.h>

char	*ft_strdup(const char *s) {
	char	*dest;
	size_t	len;
	len = (size_t)s;
	while (*s)
		s++;
	len = (size_t)s - len;
	s -= len;
	if (!(dest = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (*s)
		*dest++ = (char)*s++;
	*dest = '\0';
	dest -= len;
	return (dest);
}

