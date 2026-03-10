#ifndef AD_H
#define AD_H
#include "millis.h"

extern const int AD_COST;
extern const millis_t AD_RUNTIME_MS;
extern const int AD_COST_PER_SECOND;

enum Animation { BLINK, SCROLL, NONE };
enum AdStrategyRule { EVEN_MINUTES, ODD_MINUTES };

struct Ad {
  char *ad_text;
  enum Animation animation;
  enum AdStrategyRule ad_rule;
};

void ad_run(const struct Ad *ad, char *company_name);

// Move ad functions from LCD to here.

#endif // !AD_H
