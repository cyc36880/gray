#include "../inc/light.h"

struct LED_PIN
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pinx;
};

static const struct LED_PIN LED_PIN_X[] = //LED灯的端口和引脚
{
    {LED1_GPIO_Port, LED1_Pin},
    {LED2_GPIO_Port, LED2_Pin},
    {LED3_GPIO_Port, LED3_Pin},
    {LED4_GPIO_Port, LED4_Pin},
    {LED5_GPIO_Port, LED5_Pin},
    {LED6_GPIO_Port, LED6_Pin},
};

// 设置led状态 x=1亮,x=0灭 （倒置）
#define SetLEDSta(x) ((x) ? GPIO_PIN_SET : GPIO_PIN_RESET)


// 底部打光三色灯
void set_rgb_brightness(uint32_t r, uint32_t g, uint32_t b)
{
    TIM1->CCR1 = r;
	TIM1->CCR2 = g;
	TIM1->CCR3 = b;
}

// 地址指示灯
void set_address_rgb(uint32_t r, uint32_t g, uint32_t b)
{
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, r ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, g ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, b ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// 每路识别状态指示灯
void identify_light(MACHINE_STATE state, uint8_t * identify_data)
{
    uint8_t light_state[SENSORE_NUM] = {0}; // 0: off, 1: on
    switch (state)
    {
        case MACHINE_GRAY_IDENTIFY : 
            for (int i = 0; i < SENSORE_NUM; i++)
            {
                light_state[i] = identify_data[i] > IDENTIFY_THRESHOLD ? 1 : 0;
            }
            break;

        case MACHINE_BINARY_IDENTIFY :
            for (int i = 0; i < SENSORE_NUM; i++)
            {
                light_state[i] = identify_data[i] == 1 ? 1 : 0;
            }break;

        default :
            break;
    }
    // TODO: 设置灯的状态
    for (int i = 0; i < SENSORE_NUM; i++)
    {
        HAL_GPIO_WritePin(LED_PIN_X[i].GPIOx, LED_PIN_X[i].GPIO_Pinx, SetLEDSta(light_state[i]));
    }
}

