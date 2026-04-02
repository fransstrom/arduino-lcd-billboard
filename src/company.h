#ifndef COMPANY_H
#define COMPANY_H

#include "ad.h"
#include "stdbool.h"

enum CompanyAdStrategy { TIME_BASED, RANDOM };
enum CompanyType { CUSTOMER, OWNER };

struct Company {
  char *company_name;
  int ad_balance; // Ticking down each running add (20 sek = AD_COST)
  struct Ad *ad_collection;
  int num_ads;
  enum CompanyAdStrategy ad_strategy;
  enum CompanyType company_type;
};

// Helpers for picking company from it's total balance
struct CompanySlot {
  struct Company *company;
  int range_min;
  int range_max;
};

struct CompanySelector {
  struct CompanySlot *company_slots;
  int num_companies;
  int total_balance;
};

struct Company *company_create(char *name, int ad_balance,
                               enum CompanyAdStrategy ad_strategy);

void company_init_ad(struct Company *company);
void company_ad_charge(struct Company *company);
bool company_add_ad(struct Company *company, struct Ad *ad);
struct Company *company_get_from_selector(struct CompanySelector selector);
struct CompanySelector *
company_create_selector(struct CompanySelector *selector);

#endif // !COMPANY_H
