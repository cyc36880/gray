/*
 * @Author       : 蔡雅超 (ZIShen)
 * @LastEditors  : ZIShen
 * @Date         : 2025-02-16 11:08:41
 * @LastEditTime : 2025-07-31 19:54:01
 * @Description  : 
 * Copyright (c) 2025 Author 蔡雅超 email: 2672632650@qq.com, All Rights Reserved.
 */

#ifndef __SYS_CONFIG_AND_FLASH_H__
#define __SYS_CONFIG_AND_FLASH_H__

#include "main.h"
#include "stdlib.h"

#define SYS_CONF_MALLOC(x) malloc(x)
#define SYS_CONF_FREE(x)   free(x)

#define EEPROM_SIZE (4096)

enum SYSCONF_MEM_LIST
{
    SYSCONF_MEM_LIST_VERSION = 0,
    SYSCONF_MEM_LIST_VERIFY_KEY,
    SYSCONF_MEM_LIST_I2C_ADDR_OTHER_POS,
    SYSCONF_MEM_LIST_GRAY_PARAM,
    SYSCONF_MEM_LIST_COLOR_TAB,
    
    SYSCONF_MEM_LIST_MAX
};


typedef struct sys_config_info
{
    size_t data_size;
    size_t occupation_size;
    void* data;
    uint8_t read_only : 1;
} sys_config_info_t;


sys_config_info_t* get_sys_config_info_handle(enum SYSCONF_MEM_LIST item);
// 0:可读可写   1:只读
uint8_t get_sys_conf_is_read_only(enum SYSCONF_MEM_LIST item);
size_t get_sys_config_info_data_size(enum SYSCONF_MEM_LIST item);
size_t get_sys_config_info_occupation_size(enum SYSCONF_MEM_LIST item);

// 同步到falsh
int16_t sync_sys_config_info(void);
int16_t get_sys_config_data(enum SYSCONF_MEM_LIST item, void *data);
int16_t set_sys_config_info(enum SYSCONF_MEM_LIST item, void *data, uint32_t len);



int16_t sys_config_info_init(void);


#endif /* __SYS_CONFIG_AND_FLASH_H__ */

