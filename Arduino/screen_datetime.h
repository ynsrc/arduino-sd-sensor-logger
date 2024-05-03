
#ifndef __SCREEN_DATETIME_H__
#define __SCREEN_DATETIME_H__

#include "screen_base.h"
#include "display.h"
#include "module_rotary.h"
#include "event_manager.h"

class ScreenDateTime : public Screen {
private:
  unsigned long refresh_timer = 0;
  DateTime dateTime;
  bool isAdjusting = false;
  int adjustIndex = 0;

public:

  virtual void initialize(){
    lcd.clear();
    syncWithRtcDateTime();
    dateTime = DateTime(now);
    displayDateTime();
    refresh_timer = millis();
  }

  virtual void update() {
    if (millis() - refresh_timer > 1000) {
      displayDateTime();
      refresh_timer = millis();
    }

    if (eventManager.hasEvent(EVT_ROT_SW)) {
      if (!isAdjusting) {
        isAdjusting = true;
      } else {
        if (adjustIndex < 5) {
          adjustIndex++;    
        } else {
          isAdjusting = false;
          adjustIndex = 0;
          rtc.adjust(dateTime);
        }
      }

      displayDateTime();

      delay(ROT_SW_DELAY);

      eventManager.clearEvent(EVT_ROT_SW);
    }

    if (eventManager.hasEvent(EVT_ROT_RT)) {
      if (!isAdjusting) {
        redirectTo = rot.getDirection() == ROT_CW ? REDIRECT_NEXT : REDIRECT_PREV;
      } else {
        int delta = rot.getDirection();
        uint32_t time = dateTime.unixtime();
        switch (adjustIndex) {
          case 0: dateTime = DateTime(time + delta * 31536000); break;
          case 1: dateTime = DateTime(time + delta * 2592000); break;
          case 2: dateTime = DateTime(time + delta * 86400); break;
          case 3: dateTime = DateTime(time + delta * 3600); break;
          case 4: dateTime = DateTime(time + delta * 60); break;
          case 5: dateTime = DateTime(time + delta); break;
        }
      }

      displayDateTime();

      eventManager.clearEvent(EVT_ROT_RT);
    }
  }

  void displayDateTime() {
    char date[] = "YYYY-MM-DD";
    char time[] = "hh:mm:ss";

    if (isAdjusting) {
      dateTime.toString(date);
      dateTime.toString(time);
    } else {
      syncWithRtcDateTime();
      now.toString(date);
      now.toString(time);
    }
    
    lcd.setCursor(0, 0);
    lcd.print(date);
    lcd.setCursor(0, 1);
    lcd.print(time);

    lcd.setCursor(13,1);

    if (isAdjusting) {
      switch(adjustIndex) {
        case 0: displayPgmString(str_adj_year); break;
        case 1: displayPgmString(str_adj_month); break;
        case 2: displayPgmString(str_adj_day); break;
        case 3: displayPgmString(str_adj_hour); break;
        case 4: displayPgmString(str_adj_minute); break;
        case 5: displayPgmString(str_adj_second); break;
      }
    } else {
      lcd.print("   ");
    }
  }
};

#endif //__SCREEN_DATETIME_H__