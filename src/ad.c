#include "ad.h"
#include "lcd.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

const millis_t AD_RUNTIME_MS = 2000;
const int AD_COST_PER_SECOND = 3;
const int AD_COST = 100;

struct Ad *ad_create(char *text, enum Animation animation,
                     enum AdStrategyRule ad_rule) {
  struct Ad *ad = malloc(sizeof(struct Ad));
  if (!ad) {
    printf("failed to create ad");
  }
  ad->ad_text = text;
  ad->animation = animation;
  if (!ad_rule) {
    printf("setting ad to default");
    ad->ad_rule = DEFAULT;
  }
  ad->ad_rule = ad_rule;

  return ad;
}

void ad_lcd_scroll(const struct Ad *ad, char *company_name) {

  volatile millis_t start = millis_get();

  // Following snippet if for having first row static and second row scrolling
  lcd_set_cursor(0, 0);
  // Visa första 16 tecken
  for (int i = 0; i < 16 && company_name[i] != '\0'; i++) {
    _delay_ms(100);
    lcd_write(company_name[i]);
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
  while (millis_get() - start <= AD_RUNTIME_MS) { // Oändlig loop!
    lcd_set_cursor(0, 1);

    printf("%s millis_t: %lu\n", company_name, millis_get() - start);
    // printf("%lu\n", millis_get() - time_in_func);
    // Visa 16 tecken från aktuell offset
    for (uint8_t i = 0; i < 16; i++) {
      uint8_t idx = (offset + i) % text_len;
      lcd_write(buffer[idx]);
    }

    offset = (offset + 1) % text_len;

    // Visa 16 tecken från aktuell offset
    _delay_ms(LCD_SCROLL_DELAY);
    // Avbryt med knapp eller timer om du vill
  }
  printf("LOOP SCROLL_AD DONE\n\n");
}

void ad_lcd_blink(const struct Ad *ad) {
  volatile millis_t start = millis_get();
  bool text_visible = true;

  uint8_t text_len = 0;
  while (ad->ad_text[text_len]) {
    text_len++;
  }

  // Hitta brytpunkt vid mellanslag nära tecken 16
  uint8_t break_point = 16;
  if (text_len > 16) {
    // Sök bakåt från position 16 efter ett mellanslag
    for (int8_t i = 16; i >= 0; i--) {
      if (ad->ad_text[i] == ' ') {
        break_point = i + 1; // Rad 1 börjar efter mellanslaget
        break;
      }
    }
  }

  while (millis_get() - start <= AD_RUNTIME_MS) {
    lcd_set_cursor(0, 0);

    if (text_visible) {
      // Rad 0: tecken 0 till break_point
      for (uint8_t i = 0; i < 16; i++) {
        lcd_write(i < break_point ? ad->ad_text[i] : ' ');
      }
      // Rad 1: tecken från break_point
      lcd_set_cursor(0, 1);
      for (uint8_t i = 0; i < 16; i++) {
        uint8_t idx = break_point + i;
        lcd_write(idx < text_len ? ad->ad_text[idx] : ' ');
      }
    } else {
      for (uint8_t i = 0; i < 16; i++)
        lcd_write(' ');
      lcd_set_cursor(0, 1);
      for (uint8_t i = 0; i < 16; i++)
        lcd_write(' ');
    }

    text_visible = !text_visible;
    _delay_ms(500);
  }
}

void ad_lcd_static(const struct Ad *ad) {
  volatile millis_t start = millis_get();

  uint8_t text_len = 0;
  while (ad->ad_text[text_len]) {
    text_len++;
  }

  // Hitta brytpunkt vid mellanslag nära tecken 16
  uint8_t break_point = 16;
  if (text_len > 16) {
    // Sök bakåt från position 16 efter ett mellanslag
    for (int8_t i = 16; i >= 0; i--) {
      if (ad->ad_text[i] == ' ') {
        break_point = i + 1; // Rad 1 börjar efter mellanslaget
        break;
      }
    }
  }
  lcd_set_cursor(0, 0);

  // Rad 0: tecken 0 till break_point
  for (uint8_t i = 0; i < 16; i++) {
    lcd_write(i < break_point ? ad->ad_text[i] : ' ');
  }
  // Rad 1: tecken från break_point
  lcd_set_cursor(0, 1);
  for (uint8_t i = 0; i < 16; i++) {
    uint8_t idx = break_point + i;
    lcd_write(idx < text_len ? ad->ad_text[idx] : ' ');
  }

  //"sleep" 20 seconds
  while (millis_get() - start <= AD_RUNTIME_MS) {
  }
}

void ad_run(const struct Ad *ad, char *company_name) {
  switch (ad->animation) {
  case SCROLL:
    ad_lcd_scroll(ad, company_name);
    break;
  case BLINK:
    ad_lcd_blink(ad);
    break;
  case NONE:
    ad_lcd_static(ad);
    break;
  default:
    break;
  }
}
