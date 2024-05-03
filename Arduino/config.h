#ifndef __CONFIG_H__
#define __CONFIG_H__

#define RTC_INT_PIN 2
#define ROT_CLK_PIN 3
#define ROT_DT_PIN 4
#define ROT_SW_PIN 5
#define DHT11_DATA_PIN 6

#define LCD_I2C_ADDR 0x27

#define LCD_DELAY 2000
#define ERROR_DELAY 5000
#define STATISTICS_SHOW_DELAY 4000

#define ROT_SW_DELAY 500

#define DHT11_MIN_DELAY 1000

#define C_TO_F(x) ((x)*(9.0f/5) + 32)

#endif //__CONFIG_H__
