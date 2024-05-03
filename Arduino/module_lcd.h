#ifndef __MODULE_LCD__
#define __MODULE_LCD__

#include "config.h"

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

void initializeLcdModule() {
  lcd.init();
  lcd.display();
  lcd.backlight();
}

#endif //__MODULE_LCD