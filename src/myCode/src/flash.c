#include "../inc/flash.h"

#define MemoryAddr 0x800FC00
#define MemoryBank FLASH_BANK_1
#define MemoryPage 63

#define FLASH_BUFFER_SIZE 200
#define FLASH_START_ADDR  MemoryAddr


static uint8_t flash_buffer[FLASH_BUFFER_SIZE] = {0};

void flash_Init(void)
{

}


int flash_Read(uint32_t addr, uint8_t *data, uint32_t size)
{
    uint32_t start_addr = FLASH_START_ADDR+addr;
    for (uint32_t i=0; i<size; i++)
    {
        data[i] = *(uint8_t *)(start_addr+i);
        flash_buffer[addr+i] = data[i];
    }
    return 0;
}

int flash_Write(uint32_t addr, uint8_t *data, uint32_t size)
{
    for (uint32_t i=0; i<size; i++)
    {
        flash_buffer[addr+i] = data[i];
    }
    return 0;
}


int flash_Sync(void)
{
    //1、解锁FLASH
    HAL_FLASH_Unlock();

    //2、擦除FLASH
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.Banks = MemoryBank;    //选择存储地址所在的扇区
    f.PageAddress = MemoryAddr;//选择存储地址所在的页
    f.NbPages = 1;

    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);
    HAL_Delay(5);
    //3、对FLASH烧写

    for(uint8_t i=0; i<FLASH_BUFFER_SIZE/2; i+=2)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, MemoryAddr+i, flash_buffer[i]);
    }
    //4、锁住FLASH
    HAL_FLASH_Lock();

    return 0;
}



