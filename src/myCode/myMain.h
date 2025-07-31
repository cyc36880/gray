#ifndef __MYMAIN_H__
#define __MYMAIN_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define SENSORE_NUM    6   //传感器路数

#define SERVER_ADDRESS 0x70 //从机主地址

extern uint32_t adcVal[SENSORE_NUM]; //六路adc值

void setup(void);
void loop(void);


#ifdef __cplusplus
}
#endif


#endif /* __MYMAIN_H__ */

