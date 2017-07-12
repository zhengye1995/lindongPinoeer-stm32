#include "dataflash.h"
#include "Global.h"
#include "stm32f10x.h"
sys_dataflash_t  SaveData;

uint16_t Flash_Write_Without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite) {
    uint16_t i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;
    
    FLASH_UnlockBank1();
    while((i < iNumByteToWrite) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(uint16_t*)buf);
      i = i+2;
      iAddress = iAddress + 2;
      buf = buf + 2;
    }
    
    return iNumByteToWrite;
}
/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to write into flash
  * @retval if success return the number to write, -1 if error
  *  
  */
int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t num) 
{
	/* Unlock the Flash Bank1 Program Erase controller */
	uint32_t secpos;
	uint32_t iNumByteToWrite = num;
	uint16_t secoff;
	uint16_t secremain;  
	uint16_t i = 0;    
	uint8_t tmp[FLASH_PAGE_SIZE];
	
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
	FLASH_Unlock();
 
	secpos = iAddress & (~(FLASH_PAGE_SIZE -1 )) ;//������ַ 
	secoff = iAddress & (FLASH_PAGE_SIZE -1);     //�������ڵ�ƫ��
	secremain = FLASH_PAGE_SIZE-secoff;           //����ʣ��ռ��С 
	

	if(iNumByteToWrite<=secremain) secremain = iNumByteToWrite;//������4096���ֽ�

	while( 1 ) 
	{
		Flash_Read(secpos, tmp, FLASH_PAGE_SIZE);   //������������
		for(i=0;i<secremain;i++) 
		{       //У������
			if(tmp[secoff+i]!=0XFF)break;       //��Ҫ���� 
		}
		if(i<secremain) 
		{  //��Ҫ����
			FLASHStatus = FLASH_ErasePage(secpos); //�����������
			
			if(FLASHStatus != FLASH_COMPLETE)
				return -1;
				
			for(i=0;i<secremain;i++) 
			{   //����
				tmp[i+secoff]=buf[i];   
			}
			Flash_Write_Without_check(secpos ,tmp ,FLASH_PAGE_SIZE);//д����������  
		} 
		else 
		{
			Flash_Write_Without_check(iAddress,buf,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
		}

		if(iNumByteToWrite==secremain) //д�������
			break;
		else 
		{
			secpos += FLASH_PAGE_SIZE;
			secoff = 0;//ƫ��λ��Ϊ0 
			buf += secremain;  //ָ��ƫ��
			iAddress += secremain;//д��ַƫ��    
			iNumByteToWrite -= secremain;  //�ֽ����ݼ�
			if(iNumByteToWrite>FLASH_PAGE_SIZE) 
				secremain=FLASH_PAGE_SIZE;//��һ����������д����
			else 
				secremain = iNumByteToWrite;  //��һ����������д����
		}

	}
					
	FLASH_Lock();
	return num; 
}






/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to read from flash
  * @retval if success return the number to write, without error
  *  
  */
int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t num) 
{
	int i = 0;

	while(i < num ) 
	{
		*(buf + i) = *(__IO uint8_t*) iAddress++;
		i++;
	}
	return i;
}





void init_data_save(void)
{
	uint8_t flag = 0;
	sys_dataflash_t data_init;
	int i,j;
	
	Flash_Read(FLASH_SAVE_BASE_ADD, &flag, 1);
	if(flag != 0x10)
	{
		for(i = 0; i < 2; i ++)
		{
			for(j = 0; j < 3; j++)
			{
				data_init.mag_param[j][i] = 0;
			}
		}
		flag = 0x10;
		Flash_Write(FLASH_SAVE_BASE_ADD, &flag, 1);
		Flash_Write(FLASH_SAVE_BASE_ADD + 2, (uint8_t * )&data_init, sizeof(sys_dataflash_t));
		
	}
	//Flash_Read(FLASH_SAVE_BASE_ADD + 2, (uint8_t * )&SaveData, sizeof(sys_dataflash_t));
	data_read();
	Delay_ms(20);
}


void data_save(void)
{
	int i, j;
	for(i = 0; i < 2; i ++)
	{
		for(j = 0; j < 3; j++)
		{
			SaveData.mag_param[j][i] = mag_param[j][i];
		}
	}
	
	Flash_Write(FLASH_SAVE_BASE_ADD + 2, (uint8_t * )&SaveData, sizeof(sys_dataflash_t));
}


void data_read(void)
{
	int i , j;
	Flash_Read(FLASH_SAVE_BASE_ADD + 2, (uint8_t * )&SaveData, sizeof(sys_dataflash_t));
	
	for(i = 0; i < 2; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			mag_param[j][i] = SaveData.mag_param[j][i];
		}
	}
}


























