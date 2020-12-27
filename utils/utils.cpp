#include <sys/time.h>
#include "includes.hpp"
#include <stdint.h>

uint16_t			ft_htons(uint16_t num) {
	return ((uint16_t)((((uint16_t)(num) & 0xff00) >> 8)
				| (((uint16_t)(num) & 0x00ff) << 8)));
}

int					checkStringInt(std::string const & word) {
	size_t			len = word.length();
	for (size_t i = 0; i < len; ++i) {
		if (!std::isdigit(word[i])) {
			return 1;
		}
	}
	return 0;
}

int					checkSuspiciousSymbols(std::string const & word) {
	size_t			len = word.length();
	for (size_t i = 0; i < len; ++i) {
		if (word[i] == '/') {
			return 1;
		}
	}
	return 0;
}

std::string			toLower(std::string str) {
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}
	return str;
}

std::string			toUpper(std::string str) {
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	}
	return str;
}

