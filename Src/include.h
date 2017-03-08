#ifndef __INCLUDE_H__
#define __INCLUDE_H__
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;

#define EFFECT 	0X55
#define INVALID 	0XAA

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>
#include <stddef.h>
//#include "core_cmFunc.h"

//#include "event_groups.h"
//#include "StackMacros.h"


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include"printf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "my_it.h"
#include "tim.h"

#include "bsp.h"
#include "motor.h"
#include "key.h"

#include "flash.h"
#include "save_data.h"
#include "w5500.h"
#include "socket.h"
#include "w5500_driver.h"
#include "wizchip_conf.h"
#include "cpu.h"
#include "CRC.h"
#include "bsp.h"


#include "iap.h"

#include "ult.h" 
#include "httpd.h"
#include "httpult.h"
#include "protocol.h"
#include "communication.h"





//#include "object.h"
#endif



