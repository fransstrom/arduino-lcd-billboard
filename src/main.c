#include "billboard.h"
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  lcd_init();
  // Test 2: Bättre auto-scroll
  lcd_clear();

  struct Ad testAd;
  testAd.ad_text = "Fucksucker";
  testAd.company_name = "Fuckers200 AB ";
  testAd.balance = 9000;

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
