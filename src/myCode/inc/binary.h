#ifndef __BINARY_H__
#define __BINARY_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "../myMain.h"

extern uint8_t binaryVal[SENSORE_NUM];

void binary_init(void);
void binary_study(void);
void binary_identify(void);

#ifdef __cplusplus
}
#endif


#endif /* __BINARY_H__ */

