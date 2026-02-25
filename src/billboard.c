#include "billboard.h"
#include "lcd.h"
#include "millis.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int AD_COST = 100;
const millis_t AD_RUNTIME_MS = 10000;
const int AD_COST_PER_SECOND = 3;

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

bool billboard_add_company(struct Billboard *billboard,
                           struct Company *company) {
  assert(billboard != NULL);
  assert(company != NULL);
  struct Company *companies =
      realloc(billboard->companies,
              sizeof(struct Company) * (billboard->num_companies + 1));
  if (!companies) {
    return false;
  }
  billboard->companies = companies;
  billboard->companies[billboard->num_companies] = *company;
  billboard->num_companies++;
  return true;
}

bool billboard_remove_company(struct Billboard *billboard,
                              struct Company *company) {
  assert(billboard != NULL);
  assert(company != NULL);
  for (int i = 0; i < billboard->num_companies; i++) {
    if (billboard->companies[i].company_name == company->company_name) {
      billboard->companies[i] =
          billboard->companies[billboard->num_companies + 1];
      billboard--;
      return true;
    }
  }
  return false;
}

void company_init_ad(struct Company *company) {
  // Select company add here by random / rules.
  printf("init add for %s\n", company->company_name);
  lcd_run_add(&company->ad_collection[0], company->company_name);
  // lcd_continuous_scroll_company(company, 1);
}

void billboard_run(void) {
  struct Billboard billboard;
  billboard.num_companies = 0;
  billboard.companies = NULL;

  struct Company sverte_petter;
  sverte_petter.num_ads = 0;
  sverte_petter.company_name = "Svartepetters AB";
  sverte_petter.ad_balance = 100;
  sverte_petter.ad_collection = NULL;

  struct Company ankan;
  ankan.num_ads = 0;
  ankan.company_name = "Ankas pajer AB";
  ankan.ad_balance = 20;
  ankan.ad_collection = NULL;

  // NEW STRUCTS TO DO. Create inserts in billboard / company using mallocs
  struct Ad testAd;
  testAd.company_name = "Svartepetters AB ";
  testAd.ad_text = "Bygga svart? Call Petter, 0414-30395";
  testAd.animation = SCROLL;

  struct Ad testAd2;
  testAd2.company_name = "Ankas Pajer AB";
  testAd2.ad_text = "Hurry before Manson have eaten all the pies";
  testAd2.animation = SCROLL;

  company_add_ad(&sverte_petter, &testAd);
  company_add_ad(&ankan, &testAd2);
  billboard_add_company(&billboard, &sverte_petter);
  billboard_add_company(&billboard, &ankan);
  // Now with millis we can wrap this in a while loop and create a function
  // to randomize company ads.
  int order = 0;
  while (1) {
    lcd_clear();
    // Randomize company here.
    if (order % 2 == 0) {
      company_init_ad(&billboard.companies[0]);
      printf("balance = %d\n", sverte_petter.ad_balance);
    } else {
      company_init_ad(&billboard.companies[1]);
    }
    order++;
  }
}
