#include "billboard.h"
#include "lcd.h"
#include <assert.h>
#include <stdlib.h>

const int AD_COST = 100;
void company_add_ad(struct Company *company, struct Ad *ad) {
  assert(company != NULL);
  struct Ad *ads = realloc(company->ad_collection,
                           sizeof(struct Ad) * (company->num_ads + 1));
  company->ad_collection = ads;
  company->ad_collection[company->num_ads] = *ad;
  company->num_ads++;
}

void company_init_ad(const struct Company *company) {
  lcd_continuous_scroll_company(company, 1);
}
