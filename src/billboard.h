#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "millis.h"
#include "stdbool.h"
extern const int AD_COST;
extern const millis_t AD_RUNTIME_MS;
extern const int AD_COST_PER_SECOND;

enum Animation { BLINK, SCROLL, NONE };

struct Ad {
  char *ad_text;
  enum Animation animation;
};

struct Company {
  char *company_name;
  int ad_balance; // Ticking down each running add (20 sek = AD_COST)
  struct Ad *ad_collection;
  int num_ads;
};

struct Billboard {
  struct Company *companies;     // Set
  struct Company active_company; // To not select same company twice in a row
  int num_companies;
};

void billboard_run(void);
bool billboard_add_company(struct Billboard *billboard,
                           struct Company *company);
bool billboard_remove_company(
    struct Billboard *billboard,
    struct Company *company); // Remove if ad_balance < 0
void billboard_prep(struct Billboard *billboard);

void company_init_ad(struct Company *company);
void company_ad_charge(struct Company *company);
bool company_add_ad(struct Company *company, struct Ad *ad);

// void billboard_select_random_company(
//     const struct Billboard *billboard,
//     struct Company *company); // Exclude last company. reduce add
//                               // balance for all companies and
//                               // increase chance for the company with
//                               // the most balance percentage wise.
//                               // Then somehow random select there.
//                               // Like a dice roll or uneven coin flip
//
#endif // !BILLBOARD_H
