#ifndef __MODULE_LOADER__
#define __MODULE_LOADER__

#include "config.h"
#include "module_lcd.h"
#include "module_rtc.h"
#include "module_sd.h"
#include "module_dht.h"
#include "module_rotary.h"

void loadAllModules() {
  pinMode(ROT_CLK_PIN, INPUT);
  pinMode(ROT_DT_PIN, INPUT);
  pinMode(ROT_SW_PIN, INPUT_PULLUP);
  pinMode(RTC_INT_PIN, INPUT_PULLUP);
  pinMode(DHT11_DATA_PIN, INPUT);

  initializeLcdModule();
  
  initializeRtcModule();
  
  initializeSdModule();
  
  initializeDhtModule();
  
  displayPgmInfo(str_peripherals, str_success);
  
  rot.initialize();
  
  attachInterrupt(digitalPinToInterrupt(RTC_INT_PIN), onRtcInterrupt, FALLING);
}

#endif //__MODULE_LOADER__