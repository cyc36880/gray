#include "myMain.h"

#include "tim.h"
#include "adc.h"

#include "./inc/sys_config_and_flash.h"


typedef void (*key_func_cb)(void);

typedef enum
{
    KEY_IDLE = 0,
    KEY_ON_CLICK,
    KEY_ON_LONG,
} KEY_STATE;

typedef enum
{
    MACHINE_IDLE = 0,
    MACHINE_GRAY_IDENTIFY,
    MACHINE_BINARY_IDENTIFY,
    MACHINE_GRAY_STUDY,
    MACHINE_BINARY_STUDY,

    MACHINE_COLOR_IDENTIFY,
    MACHINE_COLOR_CLEAR_STUDY,
    MACHINE_COLOR_RED_STUDY,
    MACHINE_COLOR_GREEN_STUDY,
    MACHINE_COLOR_BLUE_STUDY,
    MACHINE_COLOR_YELLOW_STUDY,
    MACHINE_COLOR_CYAN_STUDY,
    MACHINE_COLOR_PUPRLE_STUDY,
    MACHINE_COLOR_BLACK_STUDY,
    MACHINE_COLOR_WHITE_STUDY,
} MACHINE_STATE;

typedef struct
{
    KEY_STATE state;
    key_func_cb on_click;
    key_func_cb on_long;
    uint32_t last_press_time;
    uint8_t press_flag : 1;
    uint8_t tarigger_flag : 1;
} KET_Typedef;

static void key_event_handling(void);
static void other_addr_light_color(void);


static const uint8_t iic_command_map[][2] = {
    {0x00, MACHINE_IDLE},
    {0x01, MACHINE_GRAY_IDENTIFY},
    {0x02, MACHINE_BINARY_IDENTIFY},
    {0x03, MACHINE_GRAY_STUDY},
    {0x04, MACHINE_BINARY_STUDY},

    {0x05, MACHINE_COLOR_IDENTIFY},
    {0x06, MACHINE_COLOR_CLEAR_STUDY},
    {0x07, MACHINE_COLOR_RED_STUDY},
    {0x08, MACHINE_COLOR_GREEN_STUDY},
    {0x09, MACHINE_COLOR_BLUE_STUDY},
    {0x0A, MACHINE_COLOR_YELLOW_STUDY},
    {0x0B, MACHINE_COLOR_CYAN_STUDY},
    {0x0C, MACHINE_COLOR_PUPRLE_STUDY},
    {0x0D, MACHINE_COLOR_BLACK_STUDY},
    {0x0E, MACHINE_COLOR_WHITE_STUDY},
};

static uint8_t iic_read_reg_idle_val[2] = {0, 1};
static uint8_t iic_read_reg_study_val[2] = {0, 2};

static KET_Typedef key_info = {KEY_IDLE};
static MACHINE_STATE machine_state = MACHINE_IDLE;


static void key_on_click_cb(void)
{
    machine_state = MACHINE_GRAY_STUDY;
}

static void key_on_long_cb(void)
{
    // MAX_I2C_addr_other_changle();
    other_addr_light_color();
}


void setup(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	HAL_ADCEx_Calibration_Start(&hadc1);						// 校准
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcVal, SENSORE_NUM); // 开启adc

    sys_config_info_init(); // 系统配置信息初始化

    key_info.on_click = key_on_click_cb;    
    key_info.on_long = key_on_long_cb;

    
}

void loop(void)
{
    key_event_handling();

}


static void other_addr_light_color(void)
{
    // uint8_t other_addr_pos = 0;
    // const uint16_t *other_addr_color_ptr;
    // other_addr_pos = MAX_I2C_addr_get_other();
    // other_addr_color_ptr = other_addr_color[other_addr_pos];
    // set_light_brightness(other_addr_color_ptr[0], other_addr_color_ptr[1], other_addr_color_ptr[2]);
}


// ======== KEY ========
static void key_event_handling(void)
{
    uint8_t key_press = HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin);

    // 按键按下
    if (key_press == 0)
    {
        if (key_info.press_flag == 0)
        {
            key_info.press_flag = 1;
            key_info.last_press_time = HAL_GetTick();
            key_info.state = KEY_IDLE;
            key_info.tarigger_flag = 0;
        }
        else if (key_info.state == KEY_IDLE && (HAL_GetTick() - key_info.last_press_time > 1000))
        {
            key_info.state = KEY_ON_LONG;
            if (!key_info.tarigger_flag)
            {
                key_info.tarigger_flag = 1;
                if (key_info.on_long)
                    key_info.on_long();
            }
        }
    }
    // 按键抬起
    else
    {
        if (key_info.press_flag == 1)
        {
            uint32_t press_duration = HAL_GetTick() - key_info.last_press_time;
            if (press_duration >= 30 && press_duration <= 1000)
            {
                key_info.state = KEY_ON_CLICK;
                if (!key_info.tarigger_flag)
                {
                    key_info.tarigger_flag = 1;
                    if (key_info.on_click)
                        key_info.on_click();
                }
            }
        }
        // 重置状态
        key_info.press_flag = 0;
        key_info.state = KEY_IDLE;
        key_info.tarigger_flag = 0;
    }
}
