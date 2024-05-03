#ifndef __SCREEN_RECORD_H__
#define __SCREEN_RECORD_H__

#include "screen_base.h"
#include "display.h"
#include "module_dht.h"
#include "module_rtc.h"
#include "module_rotary.h"
#include "event_manager.h"
#include "recorder.h"

class ScreenRecord : public Screen {
private:
  unsigned long refresh_timer = 0;

public:
  virtual void initialize() {
    lcd.clear();
    displayCurrentStatus();
    refresh_timer = millis();
  }
  
  virtual void update() {
    if (millis() - refresh_timer > LCD_DELAY) {
      displayCurrentStatus();
      refresh_timer = millis();
    }

    if (eventManager.hasEvent(EVT_ROT_RT)) {
      redirectTo = rot.getDirection() == ROT_CW ? REDIRECT_NEXT : REDIRECT_PREV;

      eventManager.clearEvent(EVT_ROT_RT);
    }

    if (eventManager.hasEvent(EVT_ROT_SW)) {
      if (recorder.isRecording()) {
        recorder.stopRecording();
      } else {
        recorder.startRecording();
      }

      displayCurrentStatus();

      delay(ROT_SW_DELAY);
      eventManager.clearEvent(EVT_ROT_SW);
    }
  }

  void displayCurrentStatus() {
    lcd.setCursor(0, 0);
    displayPgmString(recorder.isRecording() ? str_to_rec_stop : str_to_rec_start);
    lcd.setCursor(0, 1);
    displayPgmString(str_push_for);
    displayPgmString(str_clear);
  }
};

#endif //__SCREEN_RECORD_H__