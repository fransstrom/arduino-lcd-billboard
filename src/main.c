#include "billboard.h"
#include "lcd.h"
#include "millis.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void) {

  millis_init();
  sei();
  lcd_init();
  lcd_clear();
  init_serial();

  struct Company sverte_petter;
  sverte_petter.num_ads = 0;
  sverte_petter.company_name = "Svartepetters AB";
  sverte_petter.ad_balance = 100;
  // NEW STRUCTS TO DO. Create inserts in billboard / company using mallocs
  struct Ad testAd;
  testAd.company_name = "Svartepetters AB ";
  testAd.ad_text = "Bygga svart? Call Petter, 0414-30395";
  testAd.balance = 9000;
  struct Ad testAd2;
  testAd2.company_name = "Ankas Pajer AB";
  testAd2.ad_text = "Hurry before Manson have eaten all the pies";
  testAd2.balance = 9000;

  company_add_ad(&sverte_petter, &testAd);
  company_add_ad(&sverte_petter, &testAd2);
  // Now with millis we can wrap this in a while loop and create a function to
  // randomize company ads.
  int order = 0;
  while (1) {
    lcd_clear();
    if (order % 2 == 0) {
      company_init_ad(&sverte_petter);
    } else {
      lcd_continuous_scroll_ad(&sverte_petter.ad_collection[1], 1);
    }
    order++;
  }
  return 0;
}
