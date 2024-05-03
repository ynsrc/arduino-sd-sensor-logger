#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "module_rotary.h"

#define EVT_ROT_SW  0x01
#define EVT_ROT_RT  0x02
#define EVT_RTC_AL  0x04

class EventManager {
private:
  int events = 0;

public:

  void dispatchEvents() {
    if (isRtcInterrupted || rtc.alarmFired(1)) {
      if (rtc.alarmFired(1)) {
        syncWithRtcDateTime();
        setEvent(EVT_RTC_AL);
        rtc.clearAlarm(1);
      }

      isRtcInterrupted = false;
    }

    if (rot.getDirection() != ROT_NONE) setEvent(EVT_ROT_RT);

    if (!digitalRead(ROT_SW_PIN)) setEvent(EVT_ROT_SW);
  }

  void setEvent(int event) { events |= event; }
  void clearEvent(int event) { events &= ~event; }
  void ignoreEvents() { events = 0; }
  bool hasEvent(int event) { return events & event; }
  bool isNotEmpty() { return events != 0; }
};

EventManager eventManager;

#endif //__EVENT_MANAGER_H__