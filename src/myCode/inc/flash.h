#ifndef __FLASH_H__
#define __FLASH_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "../myMain.h"

void flash_Init(void);
int flash_Read(uint32_t addr, uint8_t *data, uint32_t size);
int flash_Write(uint32_t addr, uint8_t *data, uint32_t size);
int flash_Sync(void);

#ifdef __cplusplus
}
#endif


#endif /* __FLASH_H__ */

