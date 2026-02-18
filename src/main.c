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
  // Test 2: Bättre auto-scroll
  lcd_clear();
  init_serial();
  // NEW STRUCTS TO DO. Need pointers etc (double pointers?)
  struct Ad testAd;
  testAd.company_name = "Svartepetters AB ";
  testAd.ad_text = "Bygga svart? Call Petter, 0414-30395";
  testAd.balance = 9000;
  struct Ad testAd2;
  testAd2.company_name = "Ankas Pajer AB";
  testAd2.ad_text = "Hurry before Manson have eaten all the pies";
  testAd2.balance = 9000;

  // Now with millis we can wrap this in a while loop and create a function to
  // randomize company ads.
  lcd_continuous_scroll_ad(&testAd, 1);
  lcd_clear();
  lcd_continuous_scroll_ad(&testAd2, 1);
  return 0;
}
