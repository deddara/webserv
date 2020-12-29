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
# include <cstring>
# include <iostream>
# include <stdint.h>
# include <sys/stat.h>

std::string			my_localtime();
std::string			timeToStr(time_t);
uint16_t			ft_htons(uint16_t);
void				*ft_memcpy(void *, const void *, size_t);
char				*ft_strjoin(char *, char const *);
char				*ft_strnstr(const char *, const char *, size_t);
char				*ft_itoa(int);
char				*ft_strdup(const char *str);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
int					ft_atoi(const char *);
int					checkStringInt(std::string const &);
int					checkSuspiciousSymbols(std::string const &);
std::string			toLower(std::string str);
std::string			toUpper(std::string str);

#endif
