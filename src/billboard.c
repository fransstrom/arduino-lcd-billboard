#include "billboard.h"
#include "lcd.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const int AD_COST = 100;
const int AD_RUNTIME_SECONDS = 20;
const int AD_COST_PER_SECOND = 3;

void company_add_ad(struct Company *company, struct Ad *ad) {
  assert(company != NULL);
  struct Ad *ads = realloc(company->ad_collection,
                           sizeof(struct Ad) * (company->num_ads + 1));
  company->ad_collection = ads;
  company->ad_collection[company->num_ads] = *ad;
  company->num_ads++;
}

void company_init_ad(struct Company *company) {
  // Select company add here by random / rules.
  lcd_run_add(&company->ad_collection[0], company->company_name);
  // lcd_continuous_scroll_company(company, 1);
}

void billboard_run(void) {

  struct Company sverte_petter;
  sverte_petter.num_ads = 0;
  sverte_petter.company_name = "Svartepetters AB";
  sverte_petter.ad_balance = 100;
  sverte_petter.ad_collection = NULL;
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
  company_add_ad(&sverte_petter, &testAd2);
  // Now with millis we can wrap this in a while loop and create a function to
  // randomize company ads.
  int order = 0;
  while (1) {
    lcd_clear();
    // Randomize company here.
    if (order % 2 == 0) {
      company_init_ad(&sverte_petter);
      printf("balance = %d\n", sverte_petter.ad_balance);
      // lcd_continuous_scroll_company(&sverte_petter, 1);
      // lcd_continuous_scroll_ad(&sverte_petter.ad_collection[0], 1);
    } else {
      company_init_ad(&sverte_petter);
    }
    order++;
  }
}
