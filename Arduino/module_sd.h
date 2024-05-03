#ifndef __MODULE_SD__
#define __MODULE_SD__

#include "config.h"
#include "display.h"

#include <SD.h>

void initializeSdModule() {
  while (!SD.begin(10)) {
    displayPgmInfo(str_sd_card, str_error);
    delay(ERROR_DELAY);
  }
}

#endif //__MODULE_SD__