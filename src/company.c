/**
 * @file company.c
 * @brief Company management implementation
 *
 * Handles company creation, ad management, and weighted selection logic.
 */

#include "company.h"
#include "ad.h"
#include "lcd.h"
#include "millis_helper.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Creates a new company with given parameters
 * @param name        Company name string
 * @param ad_balance  Initial advertising balance
 * @param ad_strategy TIME_BASED or RANDOM
 * @return New company pointer, or NULL on allocation failure
 */
struct Company *company_create(char *name, int ad_balance,
                               enum CompanyAdStrategy ad_strategy) {
  assert(name != NULL);
  struct Company *company = malloc(sizeof(struct Company));
  if (company == NULL) {
    return NULL;
  }
  company->company_name = name;
  company->ad_balance = ad_balance;
  company->ad_strategy = ad_strategy;
  company->ad_collection = NULL;
  company->num_ads = 0;
  company->company_type = CUSTOMER;

  return company;
}

/**
 * @brief Adds an ad to a company's collection
 * @param company Pointer to company
 * @param ad      Ad to add
 * @return true if successful, false on allocation failure
 */
bool company_add_ad(struct Company *company, struct Ad *ad) {
  assert(company != NULL);
  assert(ad != NULL);
  company->ad_collection = realloc(company->ad_collection,
                                   sizeof(struct Ad) * (company->num_ads + 1));
  if (company->ad_collection == NULL) {
    printf("failed to allocate company ad-?\n");
    return false;
  }

  company->ad_collection[company->num_ads] = *ad;
  company->num_ads++;
  return true;
}

/**
 * @brief Charges a company for displaying an ad
 * @param company Company to charge
 *
 * Note: This function exists but is currently commented out in the call path.
 * Could be used to deduct balance when an ad is displayed.
 */
void company_ad_charge(struct Company *company) {
  company->ad_balance -= AD_COST;
}

/**
 * @brief Initializes and displays an ad for a company
 * @param company Company whose ad to display
 *
 * For TIME_BASED strategy:
 *   - Even minutes: shows ads with EVEN_MINUTES rule
 *   - Odd minutes: shows ads with ODD_MINUTES rule
 *   - Falls back to DEFAULT rule if no matching time-based ad found
 *
 * For RANDOM strategy:
 *   - Randomly selects one ad from the company's collection
 */
void company_init_ad(struct Company *company) {
  if (company == NULL || company->num_ads == 0) {
    return;
  }

  struct Ad ad = company->ad_collection[0];
  bool ad_found = false;

  if (company->ad_strategy == TIME_BASED) {
    for (int i = 0; i < company->num_ads; i++) {
      if (is_even_minute() &&
          company->ad_collection[i].ad_rule == EVEN_MINUTES) {
        ad = company->ad_collection[i];
        ad_found = true;
        break;
      } else if (!is_even_minute() &&
                 company->ad_collection[i].ad_rule == ODD_MINUTES) {
        ad = company->ad_collection[i];
        ad_found = true;
        break;
      }
    }
    if (!ad_found) {
      for (int i = 0; i < company->num_ads; i++) {
        if (company->ad_collection[i].ad_rule == DEFAULT) {
          ad = company->ad_collection[i];
          ad_found = true;
          break;
        }
      }
    }
  } else {
    int rand_index = rand() % company->num_ads;
    ad = company->ad_collection[rand_index];
    ad_found = true;
  }

  if (ad_found) {
    ad_run(&ad, company->company_name);
  }
}

struct Company *company_get_from_selector(struct CompanySelector selector) {
  int rand_num = rand() % selector.total_balance;
  printf("RANGE %d \n", rand_num);
  for (int i = 0; i < selector.num_companies; i++) {
    if (rand_num >= selector.company_slots[i].range_min &&
        rand_num < selector.company_slots[i].range_max) {
      return selector.company_slots[i].company;
    }
  }
  return selector.company_slots[0].company;
}
