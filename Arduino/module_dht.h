
#ifndef __MODULE_DHT_H__
#define __MODULE_DHT_H__

#include "config.h"
#include "display.h"

#include <DHT.h>

DHT dht(DHT11_DATA_PIN, DHT11);

float dht11_humidity, dht11_temp;
unsigned long dht11_last_time = DHT11_MIN_DELAY;

float dht11_humidity_min = 100.0f, dht11_humidity_max = 0.0f;
float dht11_temp_min = 100.0f, dht11_temp_max = 0.0f;

void syncWithDhtSensor(bool force = false) {
  if (force || millis() - dht11_last_time >= DHT11_MIN_DELAY) {
    dht11_humidity = dht.readHumidity();
    dht11_temp = dht.readTemperature();
    dht11_last_time = millis();

    if (!isnan(dht11_humidity)) {
      dht11_humidity_min = min(dht11_humidity, dht11_humidity_min);
      dht11_humidity_max = max(dht11_humidity, dht11_humidity_max);
    }

    if (!isnan(dht11_temp)) {
      dht11_temp_min = min(dht11_temp, dht11_temp_min);
      dht11_temp_max = max(dht11_temp, dht11_temp_max);
    }
  }
}

void initializeDhtModule() {
  dht.begin();

  syncWithDhtSensor(true); 

  while (isnan(dht11_humidity) || isnan(dht11_temp)) {
    displayPgmInfo(str_dht11_module, str_error);
  }
}

#endif //__MODULE_DHT_H__