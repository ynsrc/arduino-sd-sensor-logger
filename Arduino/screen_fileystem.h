#ifndef __SCREEN_FILESYSTEM_H__
#define __SCREEN_FILESYSTEM_H__

#include "screen_base.h"
#include "display.h"
#include "module_dht.h"
#include "module_rtc.h"
#include "module_rotary.h"
#include "module_sd.h"
#include "event_manager.h"
#include "recorder.h"

#define MODE_INFO 0
#define MODE_LIST_DIR 1
#define MODE_SELECT_OPERATION 3
#define MODE_READ_FILE 4

#define OP_READ_FILE 1
#define OP_DELETE_FILE 2
#define OP_CANCEL 3
#define OP_CONFIRM_DELETE 4

class ScreenFilesystem : public Screen {
private:
  int current_mode = MODE_INFO;
  int current_op = OP_READ_FILE;
  File root, entry;
  float entry_size;

public:
  virtual void initialize() {
    lcd.clear();
    displayInfo();
    current_mode = MODE_INFO;
    current_op = OP_READ_FILE;
  }
  
  virtual void update() {

    if (eventManager.hasEvent(EVT_ROT_SW)) {
      if (current_mode == MODE_INFO) {
        listRoot();
      } else if (current_mode == MODE_LIST_DIR) {
        current_mode = MODE_SELECT_OPERATION;
        current_op = OP_READ_FILE;
        lcd.setCursor(11, 1);
        displayPgmString(str_read);
      } else if (current_mode == MODE_SELECT_OPERATION) {
        if (current_op == OP_READ_FILE) {
          current_mode = MODE_READ_FILE;
          entry.seek(0);
          displayNextLines();          
        } else if (current_op == OP_DELETE_FILE) {
          current_op = OP_CONFIRM_DELETE;
          lcd.setCursor(11, 1);
          displayPgmString(str_confirm);
        } else if (current_op == OP_CONFIRM_DELETE) {
          if (SD.remove(entry.name())) {
            lcd.setCursor(0, 0);
            displayPgmString(str_sd_file_deleted);
            displayPgmString(str_clear);
            lcd.setCursor(0, 1);
            lcd.print(entry.name());
            displayPgmString(str_clear);
            delay(LCD_DELAY);
            listRoot();
          } else {
            lcd.setCursor(0, 0);
            displayPgmString(str_sd_file_delete_error);
            displayPgmString(str_clear);
            lcd.setCursor(0, 1);
            lcd.print(entry.name());
            displayPgmString(str_clear);
            delay(LCD_DELAY);
            current_mode = MODE_LIST_DIR;
            current_op = OP_READ_FILE;
            displayEntry();
          }
        } else { // OP_CANCEL
          current_mode = MODE_LIST_DIR;
          current_op = OP_READ_FILE;
          lcd.setCursor(11, 1);
          displayPgmString(str_clear);
        }
      } else { // MODE_READ_FILE
        current_mode = MODE_LIST_DIR;
        displayEntry();
      }

      delay(ROT_SW_DELAY);
      eventManager.clearEvent(EVT_ROT_SW);
    }

    if (eventManager.hasEvent(EVT_ROT_RT)) {
      
      if (current_mode == MODE_INFO) {
        if (root) root.close();
        if (entry) entry.close();
        redirectTo = rot.getDirection() == ROT_CW ? REDIRECT_NEXT : REDIRECT_PREV;
      }

      if (current_mode == MODE_LIST_DIR) {
        if (rot.getDirection() == ROT_CW) {
          openNextEntry();
          if (entry) {
            displayEntry();
          } else {
            root.rewindDirectory();
            openNextEntry();
            if (entry) {
              displayEntry();
            } else {
              displayPgmString(str_sd_empty);
              current_mode = MODE_INFO;
              displayInfo();
            }
          }
        } else {
          current_mode = MODE_INFO;
          displayInfo();
        }        
      }

      if (current_mode == MODE_SELECT_OPERATION) {
        if (rot.getDirection() == ROT_CW) {
          current_op = current_op < OP_CANCEL ? current_op + 1 : OP_READ_FILE;
        } else {
          current_op = current_op > OP_READ_FILE ? current_op -1 : OP_CANCEL;
        }
        lcd.setCursor(11, 1);
        switch (current_op) {
          case OP_READ_FILE: displayPgmString(str_read); break;
          case OP_DELETE_FILE: displayPgmString(str_delete); break;
          case OP_CANCEL: displayPgmString(str_cancel); break;
        }
      }

      if (current_mode == MODE_READ_FILE) {
        if (!entry) {
          current_mode = MODE_INFO;
          displayInfo();
        } else {
          if (rot.getDirection() == ROT_CW) {
            displayNextLines();
          } else {
            if (entry.position() >= 64) {
              entry.seek(entry.position() - 64);
              displayNextLines();
            }
          }
        }
      }

      eventManager.clearEvent(EVT_ROT_RT);
    }

  }

  void openNextEntry() {
    if (entry) { entry.close(); }
    entry = root.openNextFile();

    if (entry.isDirectory()) {
      entry.close();
      entry = root.openNextFile();
    }
  }

  void listRoot() {
    if (root) root.close();
    root = SD.open("/");
    if (root) {
      openNextEntry();
      if (entry) {
        current_mode = MODE_LIST_DIR;
        displayEntry();
      } else {
        displayPgmInfo(str_sd_empty);
        current_mode = MODE_INFO;
        displayInfo();
      }
    } else {
      displayPgmInfo(str_sd_read_error, str_sd_root_error);
    }
  }

  void displayInfo() {
    lcd.setCursor(0, 0);
    displayPgmString(str_to_list_records);
    lcd.setCursor(0, 1);
    displayPgmString(str_push_for);
    displayPgmString(str_clear);
  }

  void displayEntry() {
    lcd.setCursor(0, 0);
    lcd.print(entry.name());
    displayPgmString(str_clear);
    lcd.setCursor(0, 1);

    entry_size = entry.size();

    int size_index = 0;

    while (entry_size >= 1024.0f) {
      entry_size /= 1024.0f;
      size_index++;
    }

    lcd.print(entry_size);
    lcd.print(" ");
    if (size_index > 0) {
      lcd.print(size_index == 1 ? "k" : (size_index == 2 ? "M" : "G"));
    }
    lcd.print("B");

    displayPgmString(str_clear);
  }

  void displayNextLines() {
    int pos = 0;
    lcd.setCursor(0, 0); 

    if (entry.available()) {
      while (entry.available()) {
        char c = entry.read();
        if (pos == 16) {
          displayPgmString(str_clear);
          lcd.setCursor(0, 1);
        }      
        if (c == '\n') c = ' ';
        lcd.write(c);
        pos++;
        if (pos == 32) break;
      }
      displayPgmString(str_clear);
    } else {
      displayPgmString(str_file_eof);
      lcd.setCursor(0, 1);
      displayPgmString(str_reached);
      displayPgmString(str_clear);
    }
  }
};

#endif //__SCREEN_FILESYSTEM_H__