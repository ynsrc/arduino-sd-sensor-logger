#ifndef __SCREEN_STATISTICS_H__
#define __SCREEN_STATISTICS_H__

#include "screen_base.h"
#include "display.h"
#include "module_dht.h"
#include "module_rtc.h"
#include "module_rotary.h"
#include "event_manager.h"
#include "recorder.h"

class ScreenStatistics : public Screen {
private:
  unsigned long refresh_timer;
  int info_index;

public:
  virtual void initialize() {
    lcd.clear();
    displayStatistics();
    info_index = 0;
    refresh_timer = 0;
  }
  
  virtual void update() {
    if (millis() - refresh_timer > STATISTICS_SHOW_DELAY) {
      displayStatistics();
      info_index = info_index < 4 ? info_index + 1 : 0;
      refresh_timer = millis();
    }

    if (eventManager.hasEvent(EVT_ROT_RT)) {
      redirectTo = rot.getDirection() == ROT_CW ? REDIRECT_NEXT : REDIRECT_PREV;
      eventManager.clearEvent(EVT_ROT_RT);
    }
  }

  void displayTemp(float value) {
    lcd.print(settings.getTempUnit() == TEMP_UNIT_C ? value : C_TO_F(value));
    lcd.write(223);
    lcd.print(settings.getTempUnit() == TEMP_UNIT_C ? "C" : "F");
  }

  void displayStatistics() {
    lcd.setCursor(0, 0);

    switch (info_index) {
      case 0:
        displayPgmString(str_dht11_temp);
        lcd.setCursor(0, 1);
        displayTemp(dht11_temp_min);
        lcd.print("-");
        displayTemp(dht11_temp_max);
        displayPgmString(str_clear);
        break;

      case 1:
        displayPgmString(str_ds3231_temp);
        lcd.setCursor(0, 1);
        displayTemp(ds3231_temp_min);
        lcd.print("-");
        displayTemp(ds3231_temp_max);
        displayPgmString(str_clear);
        break;

      case 2:
        displayPgmString(str_mean_temp);
        lcd.setCursor(0, 1);
        displayTemp((ds3231_temp_min + dht11_temp_min) / 2);
        lcd.print("-");
        displayTemp((ds3231_temp_max + dht11_temp_max) / 2);
        displayPgmString(str_clear);
        break;

      case 3:
        displayPgmString(str_dht11_humidity);
        lcd.setCursor(0, 1);
        displayPgmString(str_humidity);
        lcd.print((int)dht11_humidity_min);
        lcd.print("%-");
        lcd.print((int)dht11_humidity_max);
        lcd.print("%");
        displayPgmString(str_clear);
        break;
    }
  }
};

#endif //__SCREEN_STATISTICS_H__