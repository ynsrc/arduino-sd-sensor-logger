#include "Arduino.h"
#ifndef __MODULE_ROTARY_H
#define __MODULE_ROTARY_H

#include "config.h"

#define ROT_CW 1
#define ROT_CCW -1
#define ROT_NONE 0

class RotaryEncoder {
private:
  bool clk, dt, clk_last, dt_last;
  int rot_direction;

public:
  void initialize() {
    rot_direction = ROT_NONE;
    clk = clk_last = digitalRead(ROT_CLK_PIN);
    dt = dt_last = digitalRead(ROT_DT_PIN);
  }

  void updateDirection() {
    clk = digitalRead(ROT_CLK_PIN);
    dt = digitalRead(ROT_DT_PIN);

    if (clk == clk_last && dt != dt_last) rot_direction = clk_last ^ dt ? ROT_CCW : ROT_CW;
    else rot_direction = ROT_NONE;

    clk_last = clk;
    dt_last = dt;
  }

  int getDirection() { return rot_direction; }
};

RotaryEncoder rot;

#endif //__MODULE_ROTARY_H