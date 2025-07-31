#ifndef __MYMATH_H__
#define __MYMATH_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define VAL4096_2_255(x) ((uint32_t)x*255/4095)

typedef struct 
{
    /* @param h hue [0..359]
    * @param s saturation [0..100]
    * @param v value [0..100]
    */
    uint16_t h;
    uint8_t s;
    uint8_t v;
} lv_color_hsv_t;


int max(int num1, int num2);
int min(int num1, int num2);
lv_color_hsv_t lv_color_rgb_to_hsv(uint8_t r8, uint8_t g8, uint8_t b8);
void lv_color_hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *cr, uint8_t *cg, uint8_t *cb);



#ifdef __cplusplus
}
#endif


#endif /* __MYMATH_H__ */

