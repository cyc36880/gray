/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 */


/*
 * @Author       : 蔡雅超 (ZIShen)
 * @LastEditors  : ZIShen
 * @Date         : 2025-02-21 18:04:14
 * @LastEditTime : 2025-02-26 16:22:03
 * @Description  : 
 * Copyright (c) 2025 Author 蔡雅超 email: 2672632650@qq.com, All Rights Reserved.
 */
#include "../inc/sys_config_and_flash.h"
#include "../inc/flash.h"

static const char sys_verify_version[] = "0.0.1";
// 验证密钥
static const uint32_t sys_verify_key = 0x5E5A6A6A;

static uint8_t i2c_slaveaddr_pos = 0;


static sys_config_info_t config_info_table[] = {
    {sizeof(sys_verify_version)-1,  20,                           (void *)sys_verify_version,   1}, // 验证版本
    {sizeof(uint32_t),              sizeof(uint32_t),             (void *)&sys_verify_key,      1}, // 验证密钥
    {sizeof(i2c_slaveaddr_pos),     sizeof(i2c_slaveaddr_pos),    (void *)&i2c_slaveaddr_pos,   0}, // 第二iic地址
};

static int16_t flash_init(void)
{
    flash_Init();
    return 0;
}

static uint8_t read_data_from_flash(uint32_t addr, void *data, uint32_t len)
{
    flash_Read(addr, data, len);
    return 0;
}

static uint8_t write_data_to_flash(uint32_t addr, void *data, uint32_t len)
{
    flash_Write(addr, data, len);
    return 0;
}

static int16_t flash_sync(void)
{
    flash_Sync();
    return 0;
}

int16_t sync_sys_config_info(void)
{
    flash_sync();
    return 0;
}

/**
 * @description: 获取系统配置信息句柄
 * @param {enum SYSCONF_MEM_LIST} item
 * @return {*}
 */
sys_config_info_t* get_sys_config_info_handle(enum SYSCONF_MEM_LIST item)
{
    if (item >= SYSCONF_MEM_LIST_MAX)
        return NULL;
    return &config_info_table[item];
}

/**
 * @description: 
 * @param {enum SYSCONF_MEM_LIST} item
 * @return {*}
 */
static void* get_sys_conf_data(enum SYSCONF_MEM_LIST item)
{
    sys_config_info_t *config_info = get_sys_config_info_handle(item);
    if (NULL == config_info) return NULL;

    return config_info->data;
}

/**
 * @description: 是否只读
 * @param {enum SYSCONF_MEM_LIST} item
 * @return {*} 0：可读可写；1：只读
 */
uint8_t get_sys_conf_is_read_only(enum SYSCONF_MEM_LIST item)
{
    sys_config_info_t *config_info = get_sys_config_info_handle(item);
    if (NULL == config_info) return 2;

    return config_info->read_only;
}

/**
 * @description: 得到系统配置信息的地址偏移
 * @param {enum SYSCONF_MEM_LIST} item
 * @return {*}
 */
static size_t get_sys_config_addr_offset(enum SYSCONF_MEM_LIST item)
{
    if (item >= SYSCONF_MEM_LIST_MAX)
        return 0;

    size_t addr_offset = 0;
    for (uint16_t item_count=0; item_count<item; item_count++)
    {
        sys_config_info_t *_item = get_sys_config_info_handle((enum SYSCONF_MEM_LIST)item_count);
        size_t config_size = get_sys_config_info_occupation_size((enum SYSCONF_MEM_LIST)item_count);
        addr_offset += config_size + sizeof(_item->data_size); 
    }

    return addr_offset;
}

/**
 * @description: 到系统配置信息 数据 的地址偏移
 * @param {enum SYSCONF_MEM_LIST} item
 * @return {*}
 */
static size_t get_sys_config_data_offset(enum SYSCONF_MEM_LIST item)
{
    sys_config_info_t *_item = get_sys_config_info_handle(item);

    size_t data_offset = get_sys_config_addr_offset(item) + sizeof(_item->data_size);
    return data_offset;
}


/**
 * @description: 得到系统配置信息的大小
 * @param {enum SYSCONF_MEM_LIST} item
 * @return {*}
 */
size_t get_sys_config_info_data_size(enum SYSCONF_MEM_LIST item)
{
    sys_config_info_t *config_info = get_sys_config_info_handle(item);
    if (NULL == config_info)
        return 0;
    return config_info->data_size;
}

/**
 * @description: 得到系统配置信息所占用的空间大小
 * @param {enum SYSCONF_MEM_LIST} item
 * @return {*}
 */
size_t get_sys_config_info_occupation_size(enum SYSCONF_MEM_LIST item)
{
    sys_config_info_t *config_info = get_sys_config_info_handle(item);
    if (NULL == config_info)
        return 0;
    return config_info->occupation_size;
}


/**
 * @description: 设置系统配置信息
 * @param {enum SYSCONF_MEM_LIST} item
 * @param {void} *data
 * @param {uint32_t} len
 * @return {*}
 */
int16_t set_sys_config_info(enum SYSCONF_MEM_LIST item, void *data, uint32_t len)
{
    sys_config_info_t *config_info = get_sys_config_info_handle(item);
    const size_t addr_offset = get_sys_config_data_offset(item);

    if (config_info == NULL)
        return -1;
    if (len > config_info->occupation_size)
        return -2;
    if (1 == config_info->read_only)
        return -3;
    memcpy(config_info->data, data, len);
    config_info->data_size = len;
    write_data_to_flash(get_sys_config_addr_offset(item), &config_info->data_size, sizeof(config_info->data_size));
    write_data_to_flash(get_sys_config_data_offset(item), data, len);

    return 0;
}

/**
 * @description: 验证保存在flash中的验证密钥
 * @return {*}
 */
static int16_t sys_config_version_verify(void)
{
    const uint8_t *data = (uint8_t *)get_sys_conf_data(SYSCONF_MEM_LIST_VERIFY_KEY);
    if (NULL == data)
        return -1;
    const size_t addr_offset = get_sys_config_data_offset(SYSCONF_MEM_LIST_VERIFY_KEY);
    const size_t data_size = get_sys_config_info_data_size(SYSCONF_MEM_LIST_VERIFY_KEY);

    uint8_t* version_buf = (uint8_t *)SYS_CONF_MALLOC(data_size);
    if (NULL == version_buf)
        return -2;
    read_data_from_flash(addr_offset, version_buf, data_size);
    uint8_t flag = 0;
    for (uint32_t i=0; i<data_size; i++)
    {
        if (data[i] != version_buf[i]) 
        {
            flag = 1;
            break;
        }
    }

    SYS_CONF_FREE(version_buf);

    return flag;
}



/**
 * @description: 初始化信息
 * @return {*} 0 正确获取
 */
int16_t sys_config_info_init(void)
{
    int16_t flag=0;
    if (0 != flash_init())
    {
        flag = -1;
        goto exit;
    }

    if (0 != sys_config_version_verify()) // 密钥验证失败
    {
        for (uint16_t i=0; i<SYSCONF_MEM_LIST_MAX; i++)
        {
            enum SYSCONF_MEM_LIST item = (enum SYSCONF_MEM_LIST)i;
            size_t data_size = get_sys_config_info_data_size(item);

            write_data_to_flash(get_sys_config_addr_offset(item), &data_size, get_sys_config_info_data_size(item)); // 重刷 flash
            write_data_to_flash(get_sys_config_data_offset(item), get_sys_conf_data(item), data_size); // 重刷 flash
        }
        flash_sync();

        flag = -2;
        goto exit;
    }

    // 更新系统信息

    for (uint16_t i=2; i<SYSCONF_MEM_LIST_MAX; i++)
    {
        enum SYSCONF_MEM_LIST item = (enum SYSCONF_MEM_LIST)i;
        sys_config_info_t *config_info = get_sys_config_info_handle(item);
        if (NULL == config_info)
            continue;

        read_data_from_flash(get_sys_config_addr_offset(item), &config_info->data_size, sizeof(config_info->data_size));
        read_data_from_flash(get_sys_config_data_offset(item), get_sys_conf_data(item), config_info->data_size);
    }


exit:

    return flag;
}

int16_t get_sys_config_data(enum SYSCONF_MEM_LIST item, void *data)
{
    const uint8_t *conf_data = (uint8_t *)get_sys_conf_data(item);
    if (NULL == data)
        return -1;

    const size_t data_size = get_sys_config_info_data_size(item);
    
    for (uint16_t i=0; i<data_size; i++)
    {
        ((uint8_t *)data)[i] = conf_data[i];
    }
    return 0;
}
