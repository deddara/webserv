/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  includes.hpp                                               | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/26/20 20:20:04 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 20:39:11 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#ifndef INCLUDES_HPP
# define INCLUDES_HPP
#include <iostream>
#include "vector"
#include <stdint.h>

std::string my_localtime();
uint16_t		ft_htons(uint16_t num);
char	*ft_strjoin(char *s1, char const *s2);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int	ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char    *ft_strdup(const char *str);


#endif