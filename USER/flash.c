#include "stm32f10x_flash.h"
 #include "flash.h"
 #include "ble.h"
 #include "string.h"
 #define StartServerManageFlashAddress    ((u32)0x08036000)//��д��ʼ��ַ���ڲ�flash�����洢���ַ��0x08036000��ʼ��
#define FLASH_SIZE 256 //��ѡMCU��FLASH������С(��λΪK)
 
#if FLASH_SIZE<256
#define SECTOR_SIZE 1024  //�ֽ�
#else
#define SECTOR_SIZE 2048  //�ֽ�
#endif
//��ָ����ַ��ʼд��������
void FLASH_WriteMoreData(uint32_t startAddress,uint16_t *writeData,uint16_t countToWrite)
{    
	uint32_t offsetAddress=startAddress - FLASH_BASE;               //����ȥ��0X08000000���ʵ��ƫ�Ƶ�ַ
	uint32_t sectorPosition=offsetAddress/SECTOR_SIZE;            //����������ַ������STM32F103VET6Ϊ0~255
	uint32_t sectorStartAddress=sectorPosition*SECTOR_SIZE+FLASH_BASE;    //��Ӧ�������׵�ַ
	uint16_t dataIndex;
	if(startAddress<FLASH_BASE||((startAddress+countToWrite*2)>=(FLASH_BASE + SECTOR_SIZE * FLASH_SIZE)))
	{
		return;//�Ƿ���ַ
	}
	FLASH_Unlock();         //����д����

	FLASH_ErasePage(sectorStartAddress);//�����������

	for(dataIndex=0;dataIndex<countToWrite;dataIndex++)
	{
		FLASH_ProgramHalfWord(startAddress+dataIndex*2,writeData[dataIndex]);
	}

	FLASH_Lock();//����д����
}
 
//��ȡָ����ַ�İ���(16λ����)
uint16_t FLASH_ReadHalfWord(uint32_t address)
{
	return *(__IO uint16_t*)address; 
}
 
//��ָ����ַ��ʼ��ȡ�������
void FLASH_ReadMoreData(uint32_t startAddress,uint16_t *readData,uint16_t countToRead)
{
	uint16_t dataIndex;
	for(dataIndex=0;dataIndex<countToRead;dataIndex++)
	{
		readData[dataIndex]=FLASH_ReadHalfWord(startAddress+dataIndex*2);
	}
}
struct BLEMAC ble_mac;
u8 peizhi[10] = {0};
u8 temp[500]={0};
void write_to_flash()
{
	memset(temp, 0 ,sizeof(temp));
	memcpy(temp, &ble_mac, sizeof(struct BLEMAC));
	memcpy(temp+sizeof(struct BLEMAC), peizhi, 10);
	//u16 count_len = sizeof(ptr) / 2;
	FLASH_WriteMoreData(StartServerManageFlashAddress,(u16 *)temp,250);
}
void read_from_flash()
{
	memset(temp, 0 ,sizeof(temp));
	//u16 count_len = sizeof(ptr)  / 2;
	FLASH_ReadMoreData(StartServerManageFlashAddress,(u16 *)temp,250);
	memcpy(&ble_mac, temp, sizeof(struct BLEMAC));
	memcpy(peizhi, temp+sizeof(struct BLEMAC), 10);
	for(u8 i = 0; i < 10; i++)
	{
		if(peizhi[i] == 0xff)
		{
			peizhi[i] = 1;
		}
	}
}
