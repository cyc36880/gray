#include "../inc/register.h"

static uint8_t iic_write_reg_buf[1] = {0};

Register_t iic_read_reg = {0};
Register_t iic_write_reg = {
    .reg = iic_write_reg_buf,
    .size = 1,
};


