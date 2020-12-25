#ifndef INCLUDES_HPP
# define INCLUDES_HPP
#include <iostream>
#include <cstring>
#include <stdint.h>
#include <sys/stat.h>

std::string my_localtime();
std::string modifiedTimeToStr(time_t time);
uint16_t		ft_htons(uint16_t num);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strjoin(char *s1, char const *s2);
char	*ft_strnstr(const char *big, const char *little, size_t len);


#endif
