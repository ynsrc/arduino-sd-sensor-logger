#ifndef __SCREEN_MANAGER_H__
#define __SCREEN_MANAGER_H__

#include "screen_base.h"
#include "screen_idle.h"
#include "screen_datetime.h"
#include "screen_statistics.h"
#include "screen_record.h"
#include "screen_fileystem.h"
#include "screen_settings.h"
#include "event_manager.h"

class ScreenManager {
private:
  Screen* activeScreen;
  int activeScreenId = SCREEN_IDLE;
  int lastScreenId = SCREEN_SETTINGS;

  ScreenIdle screenIdle;
  ScreenDateTime screenDateTime;
  ScreenStatistics screenStatistics;
  ScreenRecord screenRecord;
  ScreenFilesystem screenFilesystem;
  ScreenSettings screenSettings;
  
public:
  void initialize(){
    changeScreen(&screenIdle);
  }

  void update(){
    if (activeScreen != nullptr) {
      if (activeScreen->redirectTo != DONT_REDIRECT) {
        if (activeScreen->redirectTo == REDIRECT_PREV) {
           int prev = activeScreenId == 0 ? lastScreenId : activeScreenId - 1;
           activeScreen->redirectTo = prev;
        } else if (activeScreen->redirectTo == REDIRECT_NEXT) {
          int next = activeScreenId < lastScreenId ? activeScreenId + 1 : 0;
          activeScreen->redirectTo = next;
        }
        
        activeScreenId = activeScreen->redirectTo;

        switch (activeScreen->redirectTo) {
          case SCREEN_IDLE: changeScreen(&screenIdle); break;
          case SCREEN_DATETIME: changeScreen(&screenDateTime); break;
          case SCREEN_STATISTICS: changeScreen(&screenStatistics); break;
          case SCREEN_RECORD: changeScreen(&screenRecord); break;
          case SCREEN_FILESYSTEM: changeScreen(&screenFilesystem); break;
          case SCREEN_SETTINGS: changeScreen(&screenSettings); break;
        }
      } else {
        activeScreen->update();
      }
    }
  }

  void changeScreen(Screen* new_screen) {
    activeScreen = new_screen;
    activeScreen->redirectTo = DONT_REDIRECT;
    activeScreen->initialize();
    delay(100);
    eventManager.ignoreEvents();
  }
};

ScreenManager screenManager;

#endif //__SCREEN_MANAGER_H