#ifndef INCLUDES_HPP
# define INCLUDES_HPP
#include <iostream>
#include <cstring>
#include "vector"
#include <stdint.h>

std::string my_localtime();
uint16_t		ft_htons(uint16_t num);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strjoin(char *s1, char const *s2);
char	*ft_strnstr(const char *big, const char *little, size_t len);


#endif
