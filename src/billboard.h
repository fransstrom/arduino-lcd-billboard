#ifndef BILLBOARD_H
#define BILLBOARD_H

extern const int AD_COST;

enum Animation { BLINK, SCROLL };
struct Ad {
  char *company_name;
  char *ad_text;
  int balance;
};

struct Company {
  char *company_name;
  int ad_balance; // Ticking down each running add (20 sek = AD_COST)
  struct Ad *ad_collection;
  int num_ads;
};

struct Billboard {
  struct Ad *ads;
};

void company_init_ad(const struct Company *company);
void company_add_ad(struct Company *company, struct Ad *ad);

void testfunc(void);
#endif // !BILLBOARD_H
