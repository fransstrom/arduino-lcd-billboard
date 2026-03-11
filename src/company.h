#ifndef COMPANY_H
#define COMPANY_H
#include "billboard.h"

enum CompanyAdStrategy { TIME_BASED, RANDOM };

struct Company {
  char *company_name;
  int ad_balance; // Ticking down each running add (20 sek = AD_COST)
  struct Ad *ad_collection;
  int num_ads;
  enum CompanyAdStrategy ad_strategy;
};

struct Company *company_create(char *name, int ad_balance,
                               enum CompanyAdStrategy ad_strategy);

void company_init_ad(struct Company *company);
void company_ad_charge(struct Company *company);
bool company_add_ad(struct Company *company, struct Ad *ad);

#endif // !COMPANY_H
