#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "config.h"
#include "module_lcd.h"
#include "display_texts.h"

void displayPgmString(const char* str) {
   for (byte k = 0; k < strlen_P(str); k++) {
      lcd.write(pgm_read_byte_near(str + k));
   }
}

void displayPgmInfo(const char* line1 = nullptr, const char* line2 = nullptr) {
  lcd.clear();
  if (line1 != nullptr) { displayPgmString(line1); }
  if (line2 != nullptr) {
    lcd.setCursor(0, 1);
    displayPgmString(line2);
  }
  delay(LCD_DELAY);
}

void displayBanner() {
  lcd.clear();
  lcd.print(F("      YNSRC     "));
  lcd.setCursor(0, 1);
  lcd.print(F("SD SENSOR LOGGER"));
  delay(LCD_DELAY);

  lcd.clear();
}

#endif //__DISPLAY_H__