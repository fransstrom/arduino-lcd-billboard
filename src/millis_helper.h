#ifndef MILLIS_HELPER_H
#define MILLIS_HELPER_H
#include "millis.h"
#include <stdbool.h>
#include <stdint.h>
uint32_t minutes_elapsed(void);
bool is_even_minute(void);
#endif // !MILLIS_HELPER_h
