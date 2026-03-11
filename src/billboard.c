#include "ad.h"
#include "company.h"
#include "lcd.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Billboard {
  struct Company *companies;     // Set
  struct Company active_company; // To not select same company twice in a row
  int num_companies;
};

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

void billboard_prep(struct Billboard *billboard) {
  billboard->num_companies = 0;
  billboard->companies = NULL;

  struct Company *sverte_petter =
      company_create("Svartepetters AB", 900, TIME_BASED);
  struct Company *ankan = company_create("Ankas pajer AB", 200, RANDOM);

  struct Company *harry = company_create("Hederlige Harry", 500, RANDOM);
  struct Company *goofy = company_create("Detective Goofy", 250, RANDOM);

  // Allocating memory for ads in company_add_ad since Ad is never used outside
  // of company context.
  struct Ad petter = ad_create("Bygga svart? Call Petter", BLINK, EVEN_MINUTES);
  struct Ad petter2 = ad_create("Let Petter do the work!", BLINK, ODD_MINUTES);

  struct Ad anka = ad_create("Manson is eating all the pies!", NONE, DEFAULT);
  struct Ad anka2 = ad_create("Buy pies from Granny Anka", SCROLL, DEFAULT);

  struct Ad harry1 = ad_create("Buy your car at Harry's", SCROLL, DEFAULT);
  struct Ad harry2 = ad_create("Great Deals (for Harry)", NONE, DEFAULT);
  struct Ad harry3 = ad_create("Trusty Harry's cars", BLINK, DEFAULT);

  struct Ad goofy1 = ad_create("Mysteries? Call Goofy", NONE, DEFAULT);
  struct Ad goofy2 = ad_create("Goofy takes the cake!", NONE, DEFAULT);

  company_add_ad(sverte_petter, &petter);
  company_add_ad(sverte_petter, &petter2);

  company_add_ad(ankan, &anka);
  company_add_ad(ankan, &anka2);

  company_add_ad(harry, &harry1);
  company_add_ad(harry, &harry2);
  company_add_ad(harry, &harry3);

  company_add_ad(goofy, &goofy1);
  company_add_ad(goofy, &goofy2);

  billboard_add_company(billboard, sverte_petter);
  billboard_add_company(billboard, ankan);
  billboard_add_company(billboard, goofy);
  billboard_add_company(billboard, harry);
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
