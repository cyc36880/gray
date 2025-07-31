#include "../inc/light.h"

void set_rgb_brightness(uint32_t r, uint32_t g, uint32_t b)
{

}

void set_address_rgb(uint32_t r, uint32_t g, uint32_t b)
{
    
}

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
}

