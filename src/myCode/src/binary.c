#include "../inc/binary.h"

#include "../inc/gray.h"

uint8_t binaryVal[REGISTER_NUM] = {0};

void binary_init(void)
{
    
}

void binary_study(void)
{
    gary_study();
}

void binary_identify(void)
{
    gary_identify();
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        binaryVal[i] = grayVal[i] > IDENTIFY_THRESHOLD ? 1 : 0;
    }
}

