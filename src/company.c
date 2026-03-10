#include "company.h"
#include "ad.h"
#include "lcd.h"
#include "millis_helper.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bool company_add_ad(struct Company *company, struct Ad *ad) {
  assert(company != NULL);
  assert(ad != NULL);
  struct Ad *ads = realloc(company->ad_collection,
                           sizeof(struct Ad) * (company->num_ads + 1));
  if (!ads) {
    return false;
  }
  company->ad_collection = ads;
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
    printf("AD strat TIME, millis: %lu\n", millis_get());
    for (int i = 0; i < company->num_ads; i++) {
      if (is_even_minute() &&
          company->ad_collection[i].ad_rule == EVEN_MINUTES) {

        printf("EVEN MINUTE AD GO! \n");
        ad = company->ad_collection[i];
        break;
      } else if (!is_even_minute() &&
                 company->ad_collection[i].ad_rule == ODD_MINUTES) {

        printf("ODD MINUTE AD GO! \n");
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
  lcd_run_add(&ad, company->company_name);
  // lcd_continuous_scroll_company(company, 1);
}
