#include "company.h"
#include "ad.h"
#include "lcd.h"
#include "millis_helper.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
struct Company *company_create(char *name, int ad_balance,
                               enum CompanyAdStrategy ad_strategy) {

  assert(name != NULL);
  struct Company *company = malloc(sizeof(struct Company));
  if (!company) {
    free(company);
    return NULL;
  }
  company->company_name = name;
  company->ad_balance = ad_balance;
  company->ad_strategy = ad_strategy;
  company->ad_collection = NULL;
  company->num_ads = 0;

  return company;
}

bool company_add_ad(struct Company *company, struct Ad *ad) {
  assert(company != NULL);
  assert(ad != NULL);
  company->ad_collection = realloc(company->ad_collection,
                                   sizeof(struct Ad) * (company->num_ads + 1));
  if (!company->ad_collection) {
    printf("failed to allocate company ad-?\n");
    return false;
  }

  company->ad_collection[company->num_ads] = *ad;
  company->num_ads++;
  return true;
}

void company_ad_charge(struct Company *company) {
  company->ad_balance -= AD_COST;
}

void company_init_ad(struct Company *company) {
  company_ad_charge(company);
  struct Ad ad;
  if (company->ad_strategy == TIME_BASED) {
    printf("AD strat TIME\n");
    for (int i = 0; i < company->num_ads; i++) {
      if (is_even_minute() &&
          company->ad_collection[i].ad_rule == EVEN_MINUTES) {

        // printf("EVEN MINUTE AD GO! \n");
        ad = company->ad_collection[i];
        break;
      } else if (!is_even_minute() &&
                 company->ad_collection[i].ad_rule == ODD_MINUTES) {

        // printf("ODD MINUTE AD GO! \n");
        ad = company->ad_collection[i];
        break;
      }
    }

  } else {
    printf("AD strat RAND\n");
    // Select company add here by random / rules.
    int rand_index = rand() % (company->num_ads);
    ad = company->ad_collection[rand_index];
  }
  ad_run(&ad, company->company_name);
}
