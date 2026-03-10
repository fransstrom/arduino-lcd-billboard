#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "stdbool.h"

struct Billboard;
struct Company;

void billboard_run(void);
bool billboard_add_company(struct Billboard *billboard,
                           struct Company *company);
bool billboard_remove_company(
    struct Billboard *billboard,
    struct Company *company); // Remove if ad_balance < 0
void billboard_prep(struct Billboard *billboard);
#endif // !BILLBOARD_H
