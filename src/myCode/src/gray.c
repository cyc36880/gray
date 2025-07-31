#include "../inc/gray.h"

#include "../inc/myMath.h"
#include "../inc/light.h"
#include "../inc/sys_config_and_flash.h"

Grayscale_Port Grayscale_Port_param[SENSORE_NUM] = {
    // {.maxVal=4095, .minVal=0, .MedianVal=2047, .zoom=1.0},
};

static int32_t Grayscale[SENSORE_NUM] = {0};
// 识别后的n路灰度值
uint8_t grayVal[SENSORE_NUM] = {0};


void Grayscale_Init(void)
{
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        Grayscale_Port_param[i].maxVal = 4095;
        Grayscale_Port_param[i].minVal = 0;
        Grayscale_Port_param[i].MedianVal = 2047;
        Grayscale_Port_param[i].zoom = 1.0;
    }
    // get_sys_config_data(SYSCONF_MEM_LIST_GRAY_PARAM, Grayscale_Port_param);
}

//  写灰度学习的值
static void update_Grayscale_flashdata(void)
{
    Grayscale_Port Grayscale_param[SENSORE_NUM];

    // memcpy(Grayscale_param, Grayscale_Port_param, sizeof(Grayscale_Port));
    // set_sys_config_info(SYSCONF_MEM_LIST_GRAY_PARAM, Grayscale_param, SENSORE_NUM*sizeof(Grayscale_Port));
    // sync_sys_config_info();
}


/**
 * @description: 灰度学习（死循环学习）
 * @return {*}
 */
void gary_study(void)
{
    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        Grayscale_Port_param[i].maxVal = 0;
        Grayscale_Port_param[i].minVal = 4095;
    }

    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = 1000 * 5;
    while ((HAL_GetTick() - tickstart) < wait) //
    {
        set_rgb_brightness(1000, 1000, 1000);
        HAL_Delay(10);
        for(uint8_t i = 0; i < SENSORE_NUM; i++)
        {
            HAL_Delay(1);
            uint16_t adc_val = get_adc_val(i);
            if ( adc_val > Grayscale_Port_param[i].maxVal )
                Grayscale_Port_param[i].maxVal = adc_val;
            if ( adc_val < Grayscale_Port_param[i].minVal )
                Grayscale_Port_param[i].minVal = adc_val;
        }

        HAL_Delay(15); //增加训练的视觉感受
        set_rgb_brightness(0, 0, 0); //
        HAL_Delay(15); //增加训练的视觉感受
    }
    for(uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        Grayscale_Port_param[i].zoom = 4096.0f / (Grayscale_Port_param[i].maxVal - Grayscale_Port_param[i].minVal); //计算缩放比例
        Grayscale_Port_param[i].MedianVal = (Grayscale_Port_param[i].maxVal + Grayscale_Port_param[i].minVal) / 2; // 计算中值
    }
    update_Grayscale_flashdata(); //向flash写入数据
}


void gary_identify(void)
{
    set_rgb_brightness(1000, 1000, 1000);
    // SysTickDelay(1);//adc转换时间
    for(uint8_t i = 0; i < SENSORE_NUM; i++) //获取原始值
    {
        Grayscale[i] = get_adc_val(i);   
    }

    for(uint8_t i = 0; i < SENSORE_NUM; i++) 
    {
        Grayscale[i] +=  (Grayscale_Port_param[i].MedianVal - Grayscale[i])*2;//围绕中值反转
        Grayscale[i] = (Grayscale[i] - Grayscale_Port_param[i].minVal) * Grayscale_Port_param[i].zoom; //比例缩放
    }
    
    for(uint8_t i = 0; i < SENSORE_NUM; i++) // 输出灰度值
    {
        if(Grayscale[i] < 0) Grayscale[i] = 0;
        Grayscale[i] = VAL4096_2_255(Grayscale[i]);

        Grayscale[i] = Grayscale[i]<=255 ? Grayscale[i] : 255;//放大限幅
        Grayscale[i] = Grayscale[i]>=0 ? Grayscale[i] : 0;
        grayVal[i] = Grayscale[i];
    }
}
