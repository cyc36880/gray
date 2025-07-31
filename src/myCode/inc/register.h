#ifndef __REGISTER_H__
#define __REGISTER_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "../myMain.h"

typedef struct
{
    uint8_t* reg;
    uint8_t size;
    uint8_t changle_flag;
} Register_t;

extern Register_t iic_read_reg;
extern Register_t iic_write_reg;

#ifdef __cplusplus
}
#endif


#endif /* __REGISTER_H__ */

