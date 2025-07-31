#ifndef __MYMAIN_H__
#define __MYMAIN_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define SENSORE_NUM    6   //传感器路数

#define IDENTIFY_THRESHOLD (170) //灰度识别阈值
#define SERVER_ADDRESS     0x70  //从机主地址

typedef enum
{
    MACHINE_IDLE = 0,
    MACHINE_GRAY_IDENTIFY,
    MACHINE_BINARY_IDENTIFY,
    MACHINE_GRAY_STUDY,
    MACHINE_BINARY_STUDY,

    MACHINE_COLOR_IDENTIFY,
    MACHINE_COLOR_CLEAR_STUDY,
    MACHINE_COLOR_RED_STUDY,
    MACHINE_COLOR_GREEN_STUDY,
    MACHINE_COLOR_BLUE_STUDY,
    MACHINE_COLOR_YELLOW_STUDY,
    MACHINE_COLOR_CYAN_STUDY,
    MACHINE_COLOR_PUPRLE_STUDY,
    MACHINE_COLOR_BLACK_STUDY,
    MACHINE_COLOR_WHITE_STUDY,
} MACHINE_STATE;


void setup(void);
void loop(void);

// @param port 0 - SENSORE_NUM-1
uint32_t get_adc_val(uint8_t port);

#ifdef __cplusplus
}
#endif


#endif /* __MYMAIN_H__ */

