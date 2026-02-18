#include "billboard.h"
#include "lcd.h"
#include "millis.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
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

  while (1) {
    printf("%lu\n", millis_get());
  }
  lcd_continuous_scroll_ad(&testAd, 1);
  // lcd_continuous_scroll(august, 1);
  // lcd_continuous_scroll(svartpetter, 1);
  // while (1) {
  //   lcd_set_cursor(0, 1);
  //   lcd_printf("Waiting... :%d", num++);
  //
  //   _delay_ms(1000);
  // }
  return 0;
}
