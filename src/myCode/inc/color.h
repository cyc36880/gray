#ifndef __COLOR_H__
#define __COLOR_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "../myMain.h"

#include "myMath.h"

#define COLOR_STUDY_NUM  8

typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} colot_t;

typedef struct 
{
    uint8_t has_color; // 是否有颜色 1有 0没有
    lv_color_hsv_t hsv;
} color_table_t;


extern uint8_t colorVal[SENSORE_NUM];

void color_init(void);
void color_study(uint8_t color);
void color_identify(void);
void clear_color_study(void);

#ifdef __cplusplus
}
#endif


#endif /* __COLOR_H__ */

