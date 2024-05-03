#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <EEPROM.h>

#define TEMP_UNIT_C 0
#define TEMP_UNIT_F 1

#define TEMP_MODE_MEAN 0
#define TEMP_MODE_DHT11_ONLY 1
#define TEMP_MODE_DS3231_ONLY 2

#define DEFAULT_SAMPLING_RATE 30
#define DEFAULT_TEMP_UNIT TEMP_UNIT_C
#define DEFAULT_TEMP_MODE TEMP_MODE_MEAN

#define EEPROM_ADDR_INIT_STATE 1023
#define EEPROM_ADDR_SAMPLING_RATE 0
#define EEPROM_ADDR_TEMP_UNIT 1
#define EEPROM_ADDR_TEMP_MODE 2

#define EEPROM_INITIALIZED 170

class Settings {
private:
  bool is_eeprom_initialized;
  byte sampling_rate = DEFAULT_SAMPLING_RATE;
  byte temp_unit = DEFAULT_TEMP_UNIT;
  byte temp_mode = DEFAULT_TEMP_MODE;

  byte get(int addr) {
    if (!is_eeprom_initialized) initialize();
    return EEPROM.read(addr);
  }

  void set(int addr, byte value) {
    if (!is_eeprom_initialized) initialize();
    EEPROM.write(addr, value);
  }

public:
  void initialize() {
    is_eeprom_initialized = EEPROM.read(EEPROM_ADDR_INIT_STATE) == EEPROM_INITIALIZED;
    if (!is_eeprom_initialized) {
      setSamplingRate(DEFAULT_SAMPLING_RATE);
      setTempUnit(DEFAULT_TEMP_UNIT);
      setTempMode(DEFAULT_TEMP_MODE);
      set(EEPROM_ADDR_INIT_STATE, EEPROM_INITIALIZED);
      is_eeprom_initialized = true;
    } else {
      sampling_rate = get(EEPROM_ADDR_SAMPLING_RATE);
      temp_unit = get(EEPROM_ADDR_TEMP_UNIT);
      temp_mode = get(EEPROM_ADDR_TEMP_MODE);
    }
  }

  int getSamplingRate() { return sampling_rate; }
  int getTempUnit() { return temp_unit; }
  int getTempMode() { return temp_mode; }
  
  void setSamplingRate(byte value) {
    set(EEPROM_ADDR_SAMPLING_RATE, value);
    sampling_rate = value;
  }

  void setTempUnit(byte value) {
    set(EEPROM_ADDR_TEMP_UNIT, value);
    temp_unit = value;
  }

  void setTempMode(byte value) {
    set(EEPROM_ADDR_TEMP_MODE, value);
    temp_mode = value;
  }
};

Settings settings;

#endif // __SETTINGS_H__