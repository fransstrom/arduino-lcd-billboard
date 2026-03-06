#include "billboard.h"
#include "lcd.h"
#include "millis.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int AD_COST = 100;
const millis_t AD_RUNTIME_MS = 2000;
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

void company_ad_charge(struct Company *company) {
  company->ad_balance -= AD_COST;
}

uint32_t minutes_elapsed(void) { return millis_get() / 60000; }
bool is_even_minute(void) { return minutes_elapsed() % 2 == 0; }

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

void billboard_prep(struct Billboard *billboard) {
  billboard->num_companies = 0;
  billboard->companies = NULL;

  struct Company sverte_petter;
  sverte_petter.num_ads = 0;
  sverte_petter.company_name = "Svartepetters AB";
  sverte_petter.ad_balance = 1000;
  sverte_petter.ad_collection = NULL;
  sverte_petter.ad_strategy = TIME_BASED;

  struct Company ankan;
  ankan.num_ads = 0;
  ankan.company_name = "Ankas pajer AB";
  ankan.ad_balance = 200;
  ankan.ad_collection = NULL;
  ankan.ad_strategy = RANDOM;

  struct Company harry;
  harry.num_ads = 0;
  harry.company_name = "Hederlige Harry";
  harry.ad_balance = 500;
  harry.ad_collection = NULL;
  harry.ad_strategy = RANDOM;

  struct Company goofy;
  goofy.num_ads = 0;
  goofy.ad_collection = NULL;
  goofy.ad_balance = 250;
  goofy.company_name = "Detective Goofy";
  goofy.ad_strategy = RANDOM;

  // NEW STRUCTS TO DO. Create inserts in billboard / company using mallocs
  struct Ad petter;
  petter.ad_text = "Bygga svart? Call Petter";
  petter.animation = BLINK;
  petter.ad_rule = ODD_MINUTES;

  struct Ad petter2;
  petter2.ad_text = "Let Petter do the work!";
  petter2.animation = BLINK;
  petter2.ad_rule = EVEN_MINUTES;

  struct Ad anka;
  anka.ad_text = "Manson is eating all the pies!";
  anka.animation = NONE;

  struct Ad anka2;
  anka2.ad_text = "Buy pies from Granny Anka";
  anka2.animation = SCROLL;

  struct Ad harry1;
  harry1.ad_text = "Buy your car at Harry's";
  harry1.animation = SCROLL;

  struct Ad harry2;
  harry2.ad_text = "Great deals (for Harry)";
  harry2.animation = NONE;

  struct Ad harry3;
  harry3.ad_text = "Trusty Harry's cars";
  harry3.animation = BLINK;

  struct Ad goofy1;
  goofy1.ad_text = "Mysteries? Call Goofy!";
  goofy1.animation = NONE;

  struct Ad goofy2;
  goofy2.ad_text = "Goofy takes the cake!";
  goofy2.animation = NONE;

  company_add_ad(&sverte_petter, &petter);
  company_add_ad(&sverte_petter, &petter2);

  company_add_ad(&ankan, &anka);
  company_add_ad(&ankan, &anka2);

  company_add_ad(&harry, &harry1);
  company_add_ad(&harry, &harry2);
  company_add_ad(&harry, &harry3);

  company_add_ad(&goofy, &goofy1);
  company_add_ad(&goofy, &goofy2);

  billboard_add_company(billboard, &sverte_petter);
  billboard_add_company(billboard, &ankan);
  billboard_add_company(billboard, &goofy);
  billboard_add_company(billboard, &harry);
}

// Works, but need to sake the add_balance into account selecting companies
// divide all but active company ad_balance (100) and randomize that and and
// pick company that has that range.

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
    struct Company *selected_company =
        billboard_select_random_company(&billboard);
    billboard.active_company = *selected_company;
    lcd_clear();
    // Randomize company here.
    company_init_ad(selected_company);
  }
}
