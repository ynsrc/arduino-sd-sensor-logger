#ifndef __SCREEN_BASE_H__
#define __SCREEN_BASE_H__

#define DONT_REDIRECT -1
#define REDIRECT_NEXT -2
#define REDIRECT_PREV -3

#define SCREEN_IDLE 0
#define SCREEN_DATETIME 1
#define SCREEN_STATISTICS 2
#define SCREEN_RECORD 3
#define SCREEN_FILESYSTEM 4
#define SCREEN_SETTINGS 5

class Screen {
public:
  int redirectTo = DONT_REDIRECT; 
  virtual void initialize() = 0;
  virtual void update() = 0;
};

#endif //__SCREEN_BASE_H__