#ifndef __SCREEN_SETTINGS_H__
#define __SCREEN_SETTINGS_H__

#include "screen_base.h"
#include "display.h"
#include "module_dht.h"
#include "module_rtc.h"
#include "module_rotary.h"
#include "event_manager.h"
#include "recorder.h"

#define SETTING_SAMPLING_RATE 0
#define SETTING_TEMP_UNIT 1
#define SETTING_TEMP_MODE 2

class ScreenSettings : public Screen {
private:
  byte sampling_rate, temp_unit;
  int setting_index, temp_mode;
  bool is_adjusting;

public:
  virtual void initialize() {
    lcd.clear();
    is_adjusting = false;
    setting_index = SETTING_SAMPLING_RATE;
    sampling_rate = settings.getSamplingRate();
    temp_unit = settings.getTempUnit();
    temp_mode = settings.getTempMode();
    displaySettings();
  }
  
  virtual void update() {
    if (eventManager.hasEvent(EVT_ROT_SW)) {
      if (!is_adjusting) {
        is_adjusting = true;
        setting_index = SETTING_SAMPLING_RATE;
        displaySettings();
      } else if (setting_index < SETTING_TEMP_MODE) {
        setting_index++;
        displaySettings();
      } else {
        settings.setSamplingRate(sampling_rate);
        settings.setTempUnit(temp_unit);
        settings.setTempMode(temp_mode);
        is_adjusting = false;
        setting_index = SETTING_SAMPLING_RATE;
        displaySettings();
      }

      delay(ROT_SW_DELAY);
      eventManager.clearEvent(EVT_ROT_SW); 
    }

    if (eventManager.hasEvent(EVT_ROT_RT)) {
      if (!is_adjusting) {
        redirectTo = rot.getDirection() == ROT_CW ? REDIRECT_NEXT : REDIRECT_PREV;
      } else {
        int delta = rot.getDirection();
        switch (setting_index) {
          case SETTING_SAMPLING_RATE:
            sampling_rate += delta;
            if (sampling_rate < 1) sampling_rate = 60;
            else if (sampling_rate > 60) sampling_rate = 1;
          break;

          case SETTING_TEMP_UNIT:
            temp_unit = temp_unit == TEMP_UNIT_C ? TEMP_UNIT_F : TEMP_UNIT_C;
          break;

          case SETTING_TEMP_MODE:
            temp_mode += delta;
            if (temp_mode < 0) temp_mode = TEMP_MODE_DS3231_ONLY;
            else if (temp_mode > 2) temp_mode = TEMP_MODE_MEAN;
          break;
        }

        displaySettings();
      }

      eventManager.clearEvent(EVT_ROT_RT);
    }
  }

  void displaySettings() {
    lcd.setCursor(0, 0);
    displayPgmString(is_adjusting ? str_settings_info : str_settings);
    displayPgmString(str_clear);
    lcd.setCursor(0, 1);
    
    lcd.print(is_adjusting && setting_index == 0 ? ">" : " ");

    lcd.print(sampling_rate);
    if (sampling_rate < 10) lcd.print(" ");
    lcd.print(" ");

    lcd.print(is_adjusting && setting_index == 1 ? ">" : " ");
    lcd.print(temp_unit == TEMP_UNIT_C ? "C" : "F");
    lcd.print(" ");

    lcd.print(is_adjusting && setting_index == 2 ? ">" : " ");
    switch (temp_mode) {
      case TEMP_MODE_DHT11_ONLY: displayPgmString(str_dht11); break;
      case TEMP_MODE_DS3231_ONLY: displayPgmString(str_ds3231); break;
      default: displayPgmString(str_mean); break;
    }

    displayPgmString(str_clear);
  }
};

#endif // __SCREEN_SETTINGS_H__