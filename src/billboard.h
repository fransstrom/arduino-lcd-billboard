#ifndef BILLBOARD_H
#define BILLBOARD_H

enum Animation { BLINK, SCROLL };
struct Ad {
  char *company_name;
  char *ad_text;
  int balance;
};

struct Billboard {
  struct Ad *ads;
};

void testfunc(void);
#endif // !BILLBOARD_H
