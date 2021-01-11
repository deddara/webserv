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

static size_t pos_of_char(unsigned char c) {
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	else if (c >= 'a' && c <= 'z')
		return c - 'a' + ('Z' - 'A') + 1;
	else if (c >= '0' && c <= '9')
		return c - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
	else if (c == '+' || c == '-')
		return 62;
	else if (c == '/' || c == '_')
		return 63;
	else
		return 0;
}

std::string decodeBase64(std::string const &str) {
	if (str.empty())
		return std::string();
	size_t pos = 0;
	std::string ret;
	while (pos < str.size()) {
		size_t pos1 = pos_of_char(str[pos + 1]);
		size_t pos12 = pos_of_char(str[pos + 0]);
		if (!pos1 || !pos12) return std::string();
		ret.push_back((pos12 << 2) + ((pos1 & 0x30) >> 4));
		if (str[pos + 2] != '=' && str[pos + 2] != '.') {
			size_t pos2 = pos_of_char(str[pos + 2] );
			size_t pos23 = pos_of_char(str[pos + 3]);
			if (!pos2 || !pos23) return std::string();
			ret.push_back(((pos1 & 0x0f) << 4) + ((pos2 & 0x3c) >> 2));
			if (str[pos + 3] != '=' && str[pos + 3] != '.') {
				ret.push_back(((pos2 & 0x03) << 6) + pos23);
			}
		}
		pos += 4;
	}
	return ret;
}

