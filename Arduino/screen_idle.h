#ifndef __SCREEN_IDLE_H__
#define __SCREEN_IDLE_H__

#include "screen_base.h"
#include "display.h"
#include "module_dht.h"
#include "module_rtc.h"
#include "module_rotary.h"
#include "event_manager.h"
#include "recorder.h"

class ScreenIdle : public Screen {
private:
  unsigned long refresh_timer = 0;

public:
  virtual void initialize() {
    lcd.clear();
    displayCurrentDateTime();
    refresh_timer = millis();
  }
  virtual void update(){
    if (millis() - refresh_timer > LCD_DELAY) {
      displayCurrentDateTime();
      refresh_timer = millis();
    }

    if (eventManager.hasEvent(EVT_ROT_RT)) {
      redirectTo = rot.getDirection() == ROT_CW ? REDIRECT_NEXT : REDIRECT_PREV;

      eventManager.clearEvent(EVT_ROT_RT);
    }
  }
  void displayCurrentDateTime() {
    syncWithRtcDateTime();

    syncWithRtcSensor();
    syncWithDhtSensor();

    char time[] = "YYYY-MM-DD hh:mm";
    now.toString(time);
    
    lcd.setCursor(0, 0);
    lcd.print(time);
    lcd.setCursor(0, 1);

    float temp_value;

    switch (settings.getTempMode()) {
      case TEMP_MODE_DHT11_ONLY: temp_value = dht11_temp; break;
      case TEMP_MODE_DS3231_ONLY: temp_value = ds3231_temp; break;
      default: temp_value = (dht11_temp + ds3231_temp) / 2.0f; break;
    }

    if (settings.getTempUnit() == TEMP_UNIT_F) {
      temp_value = C_TO_F(temp_value);
    }

    lcd.print(temp_value);
    lcd.write(223);
    lcd.print(settings.getTempUnit() == TEMP_UNIT_C ? "C": "F");
    lcd.print(" ");
    lcd.print((int)dht11_humidity);
    lcd.print("%");

    lcd.setCursor(14, 1);
    lcd.print(recorder.isRecording() ? "SD" : "  ");
    displayPgmString(str_clear);
  }
};

#endif //__SCREEN_IDLE_H__