#include "settings.h"
#include "module_loader.h"
#include "screen_manager.h"
#include "event_manager.h"
#include "recorder.h"

void setup() {
  settings.initialize();
  loadAllModules();
  displayBanner();
  screenManager.initialize();
  recorder.initialize();
}

void loop() {
  rot.updateDirection();
  eventManager.dispatchEvents();
  screenManager.update();
  recorder.update();
  eventManager.ignoreEvents();
}
