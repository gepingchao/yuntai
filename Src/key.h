#ifndef __KEY_H__
#define __KEY_H__
#include "Include.h"

#define press 1
#define long_press 2

#define key_press(x)  (0X10 +2*(x))
#define key_long_press(x)  (0X11 +2*(x))
#define key_long_long_press(x)	(0X20+2*(x))

#define KEY_TIGGER(X)  (KEY_##X##_TRIGGER)

#define KEY_1_TRIGGER 	0X00
#define KEY_2_TRIGGER 	0X01
#define KEY_3_TRIGGER	0X02
#define KEY_4_TRIGGER 	0X03
#define KEY_5_TRIGGER 	0X04
#define KEY_6_TRIGGER 	0X05
#define KEY_7_TRIGGER 	0X06
#define KEY_8_TRIGGER 	0X07
#define KEY_9_TRIGGER 	0X08
#define KEY_10_TRIGGER 	0X09


#define KEY_LONG_BASE		0X60
#define KEY_LONG_TRIGGER(X)	(KEY_LONG_BASE+(X))	
#define KEY_1_LONG_TRIGGER 	(KEY_LONG_BASE+0)
#define KEY_2_LONG_TRIGGER 	(KEY_LONG_BASE+1)
#define KEY_3_LONG_TRIGGER	(KEY_LONG_BASE+2)
#define KEY_4_LONG_TRIGGER 	(KEY_LONG_BASE+3)
#define KEY_5_LONG_TRIGGER 	(KEY_LONG_BASE+4)
#define KEY_6_LONG_TRIGGER 	(KEY_LONG_BASE+5)
#define KEY_7_LONG_TRIGGER 	(KEY_LONG_BASE+6)
#define KEY_8_LONG_TRIGGER 	(KEY_LONG_BASE+7)
#define KEY_9_LONG_TRIGGER 	(KEY_LONG_BASE+8)
#define KEY_10_LONG_TRIGGER 	(KEY_LONG_BASE+9)

#define KEY_LONG_LONG_BASE		0X70
#define KEY_LONG_LONG_TRIGGER(X)	(KEY_LONG_LONG_BASE+(X))
#define KEY_1_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+0)
#define KEY_2_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+1)
#define KEY_3_LONG_LONG_TRIGGER	(KEY_LONG_LONG_BASE+2)
#define KEY_4_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+3)
#define KEY_5_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+4)
#define KEY_6_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+5)
#define KEY_7_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+6)
#define KEY_8_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+7)
#define KEY_9_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+8)
#define KEY_10_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+9)



#define key1_press  0X10
#define key1_long_press 0X11
#define key1_long_long_press	0X20

#define key2_press  0X12
#define key2_long_press 0X13
#define key2_long_long_press	0X22


#define key3_press  0X14
#define key3_long_press 0X15
#define key3_long_long_press 0X24


#define key4_press  0X16
#define key4_long_press 0X17
#define key4_long_long_press 0X26

#define key5_press  0X18
#define key5_long_press 0X19
#define key5_long_long_press 0X28


#define warn3_press  0X1A
#define warn3_long_press 0X1B
#define warn3_long_long_press 0X2A

#define warn4_press  0X1C
#define warn4_long_press 0X1D
#define warn4_long_long_press 0X2C


#define warn5_press  0X1E
#define warn5_long_press 0X1F
#define warn5_long_long_press 0X2E


typedef struct
{
	unsigned char key_point;
	unsigned char key_set_wifi;
	unsigned char set_wifi_delay;
	unsigned char key_ok[8];
	unsigned char key_value[8];
	unsigned char  press_time[8];
}key_data_def;
extern key_data_def rtos_key;

//#define Read_key_1 HAL_GPIO_ReadPin(key_1_GPIO_Port, key_1_Pin)

#define Read_KEY_1 HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin)
#define Read_KEY_2 HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin)
#define Read_KEY_3 HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin)
#define Read_KEY_4 GPIO_PIN_RESET
#define Read_KEY_5 GPIO_PIN_RESET

#define Read_WARN_1 GPIO_PIN_RESET
#define Read_WARN_2 GPIO_PIN_RESET
#define Read_WARN_3 GPIO_PIN_RESET
#define Read_WARN_4 GPIO_PIN_RESET
#define Read_WARN_5 GPIO_PIN_RESET






unsigned char is_cur_fan_not_safe(void);
void read_key(void);
unsigned char deal_key_value(unsigned key_value);
#endif
