#include <sys/time.h>
#include "includes.hpp"
#include <stdint.h>

uint16_t		ft_htons(uint16_t num) { return ((uint16_t)((((uint16_t)(num) & 0xff00) >> 8) | (((uint16_t)(num) & 0x00ff) << 8))); }