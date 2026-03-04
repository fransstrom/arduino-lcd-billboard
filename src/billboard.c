#include "billboard.h"
#include "lcd.h"
#include "millis.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int AD_COST = 100;
const millis_t AD_RUNTIME_MS = 5000;
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
  printf("copany name %s\n", company->company_name);
  // Select company add here by random / rules.
  lcd_run_add(&company->ad_collection[0], company->company_name);
  // lcd_continuous_scroll_company(company, 1);
}

void billboard_prep(struct Billboard *billboard) {
  billboard->num_companies = 0;
  billboard->companies = NULL;

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

  struct Company harry;
  harry.num_ads = 0;
  harry.company_name = "Hederlige Harry";
  harry.ad_balance = 100;
  harry.ad_collection = NULL;

  struct Company goofy;
  goofy.num_ads = 0;
  goofy.ad_collection = NULL;
  goofy.ad_balance = 100;
  goofy.company_name = "Detective Goofy";

  // NEW STRUCTS TO DO. Create inserts in billboard / company using mallocs
  struct Ad testAd;
  testAd.ad_text = "Bygga svart? Call Petter";
  testAd.animation = BLINK;

  struct Ad testAd2;
  testAd2.ad_text = "Manson is eating all the pies!";
  testAd2.animation = SCROLL;

  struct Ad harry1;
  harry1.ad_text = "Buy your car at Harry's";
  harry1.animation = SCROLL;

  struct Ad harry2;
  harry2.ad_text = "Great deals (for Harry)";
  harry2.animation = NONE;

  struct Ad goofy1;
  goofy1.ad_text = "goofyt";
  goofy1.animation = NONE;

  struct Ad goofy2;
  goofy2.ad_text = "goofyt2";
  goofy2.animation = NONE;

  company_add_ad(&sverte_petter, &testAd);
  company_add_ad(&ankan, &testAd2);
  company_add_ad(&harry, &harry1);
  company_add_ad(&harry, &harry2);
  company_add_ad(&goofy, &goofy1);
  company_add_ad(&goofy, &goofy2);

  billboard_add_company(billboard, &sverte_petter);
  billboard_add_company(billboard, &ankan);
  billboard_add_company(billboard, &goofy);
  billboard_add_company(billboard, &harry);
}

// Works, but need to sake the add_balance into account selecting companies
struct Company *
billboard_select_random_company(const struct Billboard *billboard) {
  struct Company *selected;
  do {
    int company_index = rand() % billboard->num_companies;
    selected = &billboard->companies[company_index];
  } while (strcmp(selected->company_name,
                  billboard->active_company.company_name) == 0);

  return selected;
}

void billboard_run(void) {
  struct Billboard billboard;
  billboard_prep(&billboard);

  while (1) {
    struct Company *selected = billboard_select_random_company(&billboard);
    billboard.active_company = *selected;
    lcd_clear();
    // Randomize company here.
    company_init_ad(selected);
  }
}
