#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  lcd_init();
  int num = 0;
  // Test 2: Bättre auto-scroll
  lcd_clear();
  char svartpetter[] =
      "Svartepetter, does this text have to be so fucking long???";
  char august[] = "August It-consultant, for your everyday it-need";
  lcd_continuous_scroll(august, 1);
  lcd_continuous_scroll(svartpetter, 1);
  // while (1) {
  //   lcd_set_cursor(0, 1);
  //   lcd_printf("Waiting... :%d", num++);
  //
  //   _delay_ms(1000);
  // }
  return 0;
}
