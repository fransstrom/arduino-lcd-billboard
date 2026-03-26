/**
 * @file billboard.c
 * @brief Billboard advertising system implementation
 *
 * Manages companies, handles weighted random company selection based on
 * advertising balance, and coordinates ad display on LCD.
 */

#include "ad.h"
#include "company.h"
#include "lcd.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

/**
 * @brief Billboard structure holding company collection
 *
 * Uses a dynamic array to store companies. The active_company field
 * tracks the last displayed company to avoid showing the same company
 * twice in a row.
 */
struct Billboard {
  struct Company *companies;     /**< Dynamic array of companies */
  struct Company active_company; /**< Last selected company */
  int num_companies;             /**< Current number of companies */
};

/**
 * @brief Adds a company to the billboard
 * @param billboard Pointer to billboard instance
 * @param company   Company to add
 * @return true if successful, false on allocation failure
 */
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

/**
 * @brief Removes a company from the billboard
 * @param billboard Pointer to billboard instance
 * @param company   Company to remove
 * @return true if found and removed, false if not found
 *
 * Removal is done by swapping with the last element and decrementing count.
 * This maintains O(1) removal but does not preserve order.
 */
bool billboard_remove_company(struct Billboard *billboard,
                              struct Company *company) {
  assert(billboard != NULL);
  assert(company != NULL);
  for (int i = 0; i < billboard->num_companies; i++) {
    if (strcmp(billboard->companies[i].company_name,
               company->company_name) == 0) {
      billboard->companies[i] =
          billboard->companies[billboard->num_companies - 1];
      billboard->num_companies--;
      return true;
    }
  }
  return false;
}

/**
 * @brief Initializes billboard with demo companies and ads
 * @param billboard Pointer to billboard to initialize
 *
 * Creates 5 demo companies with different advertising strategies:
 * - Frans Reklam AB (OWNER): Fallback company with high balance
 * - Svartepetters AB (TIME_BASED): Shows ads based on odd/even minutes
 * - Ankas pajer AB, Hederlige Harry, Detective Goofy: RANDOM strategy
 */
void billboard_prep(struct Billboard *billboard) {
  billboard->num_companies = 0;
  billboard->companies = NULL;

  struct Company *frans_billboards =
      company_create("Frans Reklam AB", 1000, RANDOM);
  frans_billboards->company_type = OWNER;
  struct Company *sverte_petter =
      company_create("Svartepetters AB", 900, TIME_BASED);
  struct Company *ankan = company_create("Ankas pajer AB", 200, RANDOM);
  struct Company *harry = company_create("Hederlige Harry", 500, RANDOM);
  struct Company *goofy = company_create("Detective Goofy", 250, RANDOM);

  // Allocating memory for ads in company_add_ad since Ad is never used outside
  // of company context.
  struct Ad frasse = ad_create("Your ad here = $$$$$", BLINK, DEFAULT);
  struct Ad frasse2 =
      ad_create("Want your ad here? call us on 0414-30395", SCROLL, DEFAULT);

  struct Ad petter = ad_create("Bygga svart? Call Petter", BLINK, EVEN_MINUTES);
  struct Ad petter2 = ad_create("Let Petter do the work!", BLINK, ODD_MINUTES);

  struct Ad anka = ad_create("Manson is eating all the pies!", NONE, DEFAULT);
  struct Ad anka2 = ad_create("Buy pies from Granny Anka", SCROLL, DEFAULT);

  struct Ad harry1 = ad_create("Buy your car at Harry's", SCROLL, DEFAULT);
  struct Ad harry2 = ad_create("Great Deals (for Harry)", NONE, DEFAULT);
  struct Ad harry3 = ad_create("Trusty Harry's cars", BLINK, DEFAULT);

  struct Ad goofy1 = ad_create("Mysteries? Call Goofy", NONE, DEFAULT);
  struct Ad goofy2 = ad_create("Goofy takes the cake!", NONE, DEFAULT);

  company_add_ad(frans_billboards, &frasse);
  company_add_ad(frans_billboards, &frasse2);

  company_add_ad(sverte_petter, &petter);
  company_add_ad(sverte_petter, &petter2);

  company_add_ad(ankan, &anka);
  company_add_ad(ankan, &anka2);

  company_add_ad(harry, &harry1);
  company_add_ad(harry, &harry2);
  company_add_ad(harry, &harry3);

  company_add_ad(goofy, &goofy1);
  company_add_ad(goofy, &goofy2);

  billboard_add_company(billboard, frans_billboards);
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

/**
 * @brief Builds a weighted company selector for random selection
 * @param billboard Source billboard with companies
 * @param selector  Output selector to populate
 *
 * Algorithm: Creates slots where each company's weight equals their ad_balance.
 * Example: If balances are [100, 200, 50], ranges become [0-100], [100-300], [300-350].
 * A random number in [0, 350) selects a company proportionally to their balance.
 * Only companies with sufficient balance (>= AD_COST) and not the active company
 * are included in the selector.
 */
void billboard_build_company_selector(const struct Billboard *billboard,
                                      struct CompanySelector *selector) {
  selector->num_companies = 0;
  selector->total_balance = 0;
  selector->company_slots =
      malloc(sizeof(struct CompanySlot) * billboard->num_companies);
  struct Company *company;
  // todo make this cleaner
  for (int i = 0; i < billboard->num_companies; i++) {
    company = &billboard->companies[i];
    if (company->ad_balance >= AD_COST &&
        strcmp(company->company_name,
               billboard->active_company.company_name) != 0) {
      struct CompanySlot company_slot;
      company_slot.company = company;
      if (selector->num_companies == 0) {
        company_slot.range_min = 0;
        company_slot.range_max = company->ad_balance;
      } else {
        company_slot.range_min = selector->total_balance;
        company_slot.range_max = company->ad_balance + selector->total_balance;
      }
      selector->company_slots[selector->num_companies] = company_slot;
      selector->total_balance += company->ad_balance;
      selector->num_companies++;
    }
  }
}

/**
 * @brief Selects a company to display based on weighted random selection
 * @param billboard Billboard to select from
 * @return Selected company, falls back to OWNER company if no others qualify
 *
 * Builds a company selector using billboard_build_company_selector, then
 * uses company_get_from_selector to perform weighted random selection.
 * If no companies have sufficient balance, returns the OWNER company as fallback.
 */
struct Company *billboard_select_company(const struct Billboard *billboard) {
  struct CompanySelector selector;
  struct Company *selected_company;
  billboard_build_company_selector(billboard, &selector);
  //     selector.num_companies++;
  // Falling back to owner company all balance is insufficient
  if (selector.num_companies == 0) {
    // select to show ad for billboard company
    for (int i = 0; i < billboard->num_companies; i++) {
      if (billboard->companies[i].company_type == OWNER) {
        return &billboard->companies[i];
      }
    }
  }

  printf("Total balance: %d\n", selector.total_balance);
  printf("NUM COMPANIES: %d\n", selector.num_companies);
  for (int i = 0; i < selector.num_companies; i++) {
    printf("Company: %s\n Range: %d - %d\n",
           selector.company_slots[i].company->company_name,
           selector.company_slots[i].range_min,
           selector.company_slots[i].range_max);
  }

  selected_company = company_get_from_selector(selector);
  free(selector.company_slots);
  return selected_company;
}

/**
 * @brief Main billboard loop - runs indefinitely
 *
 * Initializes LCD and billboard, then enters infinite loop:
 * 1. Selects a company using weighted random selection
 * 2. Stores it as active_company
 * 3. Clears LCD
 * 4. Initializes and runs the ad display
 */
void billboard_run(void) {
  lcd_init();
  lcd_clear();

  struct Billboard billboard;
  billboard_prep(&billboard);

  while (1) {
    struct Company *selected_company = billboard_select_company(&billboard);
    billboard.active_company = *selected_company;
    lcd_clear();
    // Randomize company here.
    company_init_ad(selected_company);
  }
}
