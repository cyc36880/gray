#ifndef __GRAY_H__
#define __GRAY_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "../myMain.h"

typedef struct 
{
   int32_t maxVal;
   int32_t minVal;
   int32_t MedianVal;
   float zoom;
}Grayscale_Port;


extern uint8_t grayVal[REGISTER_NUM];

void Grayscale_Init(void);
void gary_study(void);
void gary_identify(void);

#ifdef __cplusplus
}
#endif


#endif /* __GRAY_H__ */

