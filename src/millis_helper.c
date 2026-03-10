#include "millis.h"
#include <stdbool.h>
#include <stdint.h>
uint32_t minutes_elapsed(void) { return millis_get() / 60000; }
bool is_even_minute(void) { return minutes_elapsed() % 2 == 0; }
