#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/types.h>

void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals);
size_t simple_time_format(char *buf, size_t buf_sz, time_t time);

#endif
