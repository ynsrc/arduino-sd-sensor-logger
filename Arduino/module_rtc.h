#ifndef __MODULE_RTC_H__
#define __MODULE_RTC_H__

#include "config.h"
#include "display.h"

#include <RTClib.h>

RTC_DS3231 rtc;

DateTime now;
float ds3231_temp;

float ds3231_temp_min = 100.0f, ds3231_temp_max = 0.0f;

volatile bool isRtcInterrupted;
void onRtcInterrupt() { isRtcInterrupted = true; }

void syncWithRtcDateTime() {
  now = rtc.now();
}

void syncWithRtcSensor() {
  ds3231_temp = rtc.getTemperature();
  
  if (!isnan(ds3231_temp)) {
    ds3231_temp_min = min(ds3231_temp, ds3231_temp_min);
    ds3231_temp_max = max(ds3231_temp, ds3231_temp_max);
  }
}

void initializeRtcModule() {
  while (!rtc.begin()) {
    displayPgmInfo(str_rtc_module, str_error);
    delay(ERROR_DELAY);
  }
  
  rtc.disable32K();
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  
  rtc.writeSqwPinMode(DS3231_OFF);
  
  rtc.disableAlarm(2);
  
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    displayPgmInfo(str_clock, str_not_set);
  }
  
  syncWithRtcSensor();
  syncWithRtcDateTime();

  rtc.setAlarm1(now, Ds3231Alarm1Mode::DS3231_A1_PerSecond);
}

#endif //__MODULE_RTC_H__