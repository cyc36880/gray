#ifndef __LIGHT_H__
#define __LIGHT_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "../myMain.h"

void set_rgb_brightness(uint32_t r, uint32_t g, uint32_t b);
void set_address_rgb(uint32_t r, uint32_t g, uint32_t b);
void identify_light(MACHINE_STATE state, uint8_t * identify_data);

#ifdef __cplusplus
}
#endif


#endif /* __LIGHT_H__ */

