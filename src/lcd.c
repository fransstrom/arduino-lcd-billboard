#include "lcd.h"
#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>

void lcd_send(uint8_t value, uint8_t mode);
void lcd_write_nibble(uint8_t nibble);

static uint8_t lcd_displayparams;
static char lcd_buffer[LCD_COL_COUNT + 1];

void lcd_command(uint8_t command) { lcd_send(command, 0); }

void lcd_write(uint8_t value) { lcd_send(value, 1); }

void lcd_send(uint8_t value, uint8_t mode) {
  if (mode) {
    LCD_PORT = LCD_PORT | (1 << LCD_RS);
  } else {
    LCD_PORT = LCD_PORT & ~(1 << LCD_RS);
  }

  lcd_write_nibble(value >> 4);
  lcd_write_nibble(value);
}

void lcd_write_nibble(uint8_t nibble) {
  LCD_PORT =
      (LCD_PORT & 0xff & ~(0x0f << LCD_D4)) | ((nibble & 0x0f) << LCD_D4);

  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
  LCD_PORT = LCD_PORT | (1 << LCD_EN);
  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
  _delay_ms(0.3); // If delay less than this value, the data is not correctly
                  // displayed
}

void lcd_init(void) {
  // Configure pins as output
  LCD_DDR = LCD_DDR | (1 << LCD_RS) | (1 << LCD_EN) | (1 << LCD_D4) |
            (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

  // Choose output PORT
  LCD_PORT = LCD_PORT & ~(1 << LCD_EN) & ~(1 << LCD_RS);

  // Wait for LCD to become ready (docs say 15ms+)
  _delay_ms(50);

  lcd_write_nibble(0x03); // Switch to 4 bit mode
  _delay_ms(4.1);

  lcd_write_nibble(0x03); // 2nd time
  _delay_ms(4.1);

  lcd_write_nibble(0x03); // 3rd time
  _delay_ms(4.1);

  lcd_write_nibble(0x02); // Set 4-bit mode
  _delay_ms(4.1);

  lcd_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
  _delay_ms(4.1);

  lcd_displayparams = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_on(void) {
  lcd_displayparams |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_off(void) {
  lcd_displayparams &= ~LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_clear(void) {
  lcd_command(LCD_CLEARDISPLAY);
  _delay_ms(2);
}

void lcd_return_home(void) {
  lcd_command(LCD_RETURNHOME);
  _delay_ms(2);
}

void lcd_enable_blinking(void) {
  lcd_displayparams |= LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_blinking(void) {
  lcd_displayparams &= ~LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_enable_cursor(void) {
  lcd_displayparams |= LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_cursor(void) {
  lcd_displayparams &= ~LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_scroll_left(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scroll_right(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_set_left_to_right(void) {
  lcd_displayparams |= LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_set_right_to_left(void) {
  lcd_displayparams &= ~LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_enable_autoscroll(void) {
  lcd_displayparams |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_disable_autoscroll(void) {
  lcd_displayparams &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_create_char(uint8_t location, uint8_t *charmap) {
  lcd_command(LCD_SETCGRAMADDR | ((location & 0x7) << 3));
  for (int i = 0; i < 8; i++) {
    lcd_write(charmap[i]);
  }
  lcd_command(LCD_SETDDRAMADDR);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
  static uint8_t offsets[] = {0x00, 0x40, 0x14, 0x54};

  lcd_command(LCD_SETDDRAMADDR | (col + offsets[row]));
}

void swedish_parser(char *text) {
  char *src = text;
  char *dst = text;

  while (*src) {
    // Check for UTF-8 encoded Swedish characters (0xC3 xx)
    if ((unsigned char)src[0] == 0xC3) {
      switch ((unsigned char)src[1]) {
      case 0xA5: // å
        *dst++ = (char)0xE5;
        src += 2;
        continue;

      case 0xA4: // ä
        *dst++ = (char)0xE4;
        src += 2;
        continue;

      case 0xB6: // ö
        *dst++ = (char)0xF6;
        src += 2;
        continue;

      // STORA bokstäver - KORRIGERADE KODER!
      case 0x85:             // Å
        *dst++ = (char)0xEE; // Ändrat från 0xC5
        src += 2;
        continue;

      case 0x84:             // Ä
        *dst++ = (char)0xEF; // Ändrat från 0xC4
        src += 2;
        continue;

      case 0x96:             // Ö
        *dst++ = (char)0xF5; // Ändrat från 0xD6
        src += 2;
        continue;
      }
    }

    // Normal character copy
    *dst++ = *src++;
  }
  *dst = '\0';
}
void lcd_puts(char *string) {
  swedish_parser(string);
  for (char *it = string; *it; it++) {
    lcd_write(*it);
  }
}

void lcd_printf(char *format, ...) {
  // Tecken är hårdvaruprogrammerat? Detta funkar inte förutom på wokwi?
  swedish_parser(format);
  va_list args;

  va_start(args, format);
  vsnprintf(lcd_buffer, LCD_COL_COUNT + 1, format, args);
  va_end(args);

  lcd_puts(lcd_buffer);
}

void lcd_autoscroll_text(char *text, uint8_t row) {
  uint8_t text_len = 0;
  while (text[text_len])
    text_len++;

  if (text_len <= 16) {
    // Text passar, visa bara
    lcd_set_cursor(0, row);
    lcd_puts(text);
    return;
  }

  // Visa hela texten genom att scrolla
  lcd_set_cursor(0, row);

  // Visa första 16 tecken
  for (int i = 0; i < 16; i++) {
    lcd_write(text[i]);
  }

  _delay_ms(LCD_SCROLL_DELAY * 2); // Pausa i början
  // Skriv resterande tecken med display shift
  for (int i = 16; i < text_len; i++) {
    lcd_scroll_left();       // Shifta display
    lcd_set_cursor(15, row); // Gå till sista kolumnen
    lcd_write(text[i]);      // Skriv nytt tecken
    _delay_ms(LCD_SCROLL_DELAY);
  }

  _delay_ms(LCD_SCROLL_DELAY * 2); // Pausa i slutet
}

void lcd_continuous_scroll(char *text, uint8_t row) {
  uint8_t text_len = 0;
  while (text[text_len])
    text_len++;

  // Padding för att separera start/slut
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "%s    ", text);
  text_len += 4;

  uint8_t offset = 0;

  while (1) { // Oändlig loop!
    lcd_set_cursor(0, row);

    // Visa 16 tecken från aktuell offset
    for (uint8_t i = 0; i < 16; i++) {
      uint8_t idx = (offset + i) % text_len;
      lcd_write(buffer[idx]);
    }

    offset = (offset + 1) % text_len;

    // Following snippet if for having first row static and second row scrolling
    //  lcd_set_cursor(0, 0);
    //
    //  // Visa 16 tecken från aktuell offset
    //  for (uint8_t i = 0; i < 16; i++) {
    //    uint8_t idx = (0 + i) % text_len;
    //    lcd_write(buffer[idx]);
    //  }

    // Visa 16 tecken från aktuell offset
    _delay_ms(LCD_SCROLL_DELAY);

    // Avbryt med knapp eller timer om du vill
  }
}
void lcd_continuous_scroll_ad(struct Ad *ad, uint8_t row) {

  // while (ad->company_name[company_name_len]) {
  //   company_name_len++;
  // }
  // Following snippet if for having first row static and second row scrolling
  lcd_set_cursor(0, 0);

  // Visa 16 tecken från aktuell offset
  // for (uint8_t i = 0; i < 16; i++) {
  //   uint8_t idx = (0 + i) % company_name_len;
  //   lcd_write(buffer[idx]);
  // }

  // Visa första 16 tecken
  for (int i = 0; i < 16 && ad->company_name[i] != '\0'; i++) {
    _delay_ms(100);
    lcd_write(ad->company_name[i]);
  }

  uint8_t text_len = 0;
  char buffer[100];
  while (ad->ad_text[text_len]) {
    text_len++;
  };

  // Padding för att separera start/slut
  snprintf(buffer, sizeof(buffer), "%s    ", ad->ad_text);
  text_len += 4;

  uint8_t offset = 0;
  while (1) { // Oändlig loop!
    lcd_set_cursor(0, row);

    // Visa 16 tecken från aktuell offset
    for (uint8_t i = 0; i < 16; i++) {
      uint8_t idx = (offset + i) % text_len;
      lcd_write(buffer[idx]);
    }

    offset = (offset + 1) % text_len;

    // Visa 16 tecken från aktuell offset
    _delay_ms(LCD_SCROLL_DELAY);

    // Avbryt med knapp eller timer om du vill
    printf("yooyo\n");
  }
}
