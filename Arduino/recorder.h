#include "SD.h"
#ifndef __RECORDER_H__
#define __RECORDER_H__

#include "config.h"
#include "module_sd.h"
#include "event_manager.h"
#include "module_dht.h"
#include "module_rtc.h"
#include "display.h"
#include "settings.h"

#define REC_ERROR -1
#define REC_STOPPED 0
#define REC_RECORDING 1

class Recorder {
private:
  File current_file;
  unsigned long last_record_time = 0;
  unsigned long delay_between_records;
  int status = REC_STOPPED;

public:
  void initialize() {
    delay_between_records = 1000 * (60 / settings.getSamplingRate());
  }

  void update() {
    if (status == REC_RECORDING && millis() - last_record_time >= delay_between_records) {

        if (current_file) {
          syncWithRtcSensor();
          syncWithDhtSensor();

          char time[] = "MM-DD hh:mm:ss  ";
          rtc.now().toString(time);

          float temp_value;

          switch (settings.getTempMode()) {
            case TEMP_MODE_DHT11_ONLY: temp_value = dht11_temp; break;
            case TEMP_MODE_DS3231_ONLY: temp_value = ds3231_temp; break;
            default: temp_value = (dht11_temp + ds3231_temp) / 2.0f; break;
          }

          if (settings.getTempUnit() == TEMP_UNIT_F) {
            temp_value = C_TO_F(temp_value);
          }

          current_file.print(time);
          current_file.print(temp_value);
          current_file.print(settings.getTempUnit() == TEMP_UNIT_C ? "C": "F");
          current_file.print(temp_value < 10.0f ? "   " : (temp_value < 100.0f ? "  " : " "));
          current_file.print(dht11_humidity);
          current_file.print("%");
          if (dht11_humidity < 100.0f) { lcd.print(" "); }
          current_file.print(" \n");

          if (current_file.getWriteError()) {
            status = REC_ERROR;
          }

        } else {
          status = REC_ERROR;
        }

        if (status == REC_ERROR) {
          displayPgmInfo(str_sd_write_error, str_rec_stopped);
        }

        if (status == REC_RECORDING) {
          last_record_time = millis();
        }
      }
  }

  bool isRecording() { return status == REC_RECORDING; }
  
  int getStatus() { return status; }

  void startRecording() {
    if (status != REC_RECORDING) {
      char fileName[] = "MMDDhhmm.TXT";
      rtc.now().toString(fileName);
      current_file = SD.open(fileName, FILE_WRITE);
      if (!current_file) {
        status = REC_ERROR;
        displayPgmInfo(str_sd_write_error, str_rec_stopped);
      } else {
        delay_between_records = 1000 * (60 / settings.getSamplingRate());
        status = REC_RECORDING;
      }
    }
  }

  void stopRecording() {
    if (status == REC_RECORDING) {
      status = REC_STOPPED;
      current_file.close();
    }
  }
};

Recorder recorder;

#endif //__RECORDER_H__