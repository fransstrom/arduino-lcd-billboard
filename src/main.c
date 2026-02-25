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
  billboard_run();
  return 0;
}
