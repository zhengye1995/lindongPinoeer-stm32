#ifndef _DATAFLASH_H__
#define _DATAFLASH_H__

#include "Global.h"

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)
  
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

#define FLASH_SAVE_BASE_ADD	0x08010000


typedef struct
{
	float mag_param[3][2];
}sys_dataflash_t;


extern sys_dataflash_t  SaveData;

int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) ;
int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite);
void init_data_save(void);

void data_save(void);
void data_read(void);


#endif

