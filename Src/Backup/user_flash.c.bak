#include "stm32f7xx_hal.h"

extern uint8_t ipaddr[4];

#define SETTINGS_SIZE 		24
extern uint16_t SETTINGS [SETTINGS_SIZE];

#define SETTINGS_IP_ADDRES 	0x08010000	//FLASH_SECTOR_2
#define SETTINGS_MX_ADDRES 	0x08018000	//FLASH_SECTOR_3

void MX_FLASH_DefaultIP(void) {
	ipaddr[0] = 192;
	ipaddr[1] = 168;
	ipaddr[2] = 0;
	ipaddr[3] = 222;
}


void MX_FLASH_SaveIP(void) {
	HAL_FLASH_Unlock();
	FLASH_Erase_Sector(FLASH_SECTOR_2, VOLTAGE_RANGE_3);
	for (uint8_t i=0; i<sizeof(ipaddr); i++) HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, SETTINGS_IP_ADDRES+i, ipaddr[i]);
	HAL_FLASH_Lock();
}


void MX_FLASH_LoadIP(void) {
	MX_FLASH_DefaultIP();
	if (*(uint32_t*)(SETTINGS_IP_ADDRES) == 0xFFFFFFFF) MX_FLASH_SaveIP();
	else for (uint8_t i=0; i<sizeof(ipaddr); i++) ipaddr[i] = *(uint8_t*)(SETTINGS_IP_ADDRES+i);
}


void MX_FLASH_DefaultSettings(void) {
	SETTINGS[0] = 200;			//	Measuring Frequency
	SETTINGS[1] = 7000; 		//	Nakal Maximum Value
	SETTINGS[2] = 5000; 		//	Nakal Manual Value
	SETTINGS[3] = 10000; 		//	Anode Coefficient
	
	SETTINGS[4] = 30000; 		//	Coefficient U+
	SETTINGS[5] = 30000; 		//	Coefficient U-
	SETTINGS[6] = 104; 			//	Coefficient Ia
	SETTINGS[7] = 256; 			//	Coefficient Iz
	
	SETTINGS[8] = 150; 			//	Minimum Voltage
	SETTINGS[9] = 350; 			//	Maximum Voltage
	SETTINGS[10] = 60; 			//	Charging Time
	SETTINGS[11] = 30; 			//	Discharge Time
	
	SETTINGS[12] = 50; 			//	Warning Temperature
	SETTINGS[13] = 75; 			//	Error Temperature
	SETTINGS[14] = 6000; 		//	PWM Pulse
	SETTINGS[15] = 12000; 	//	PWM Period
	
	SETTINGS[16] = 50; 			//	KV Minimum
	SETTINGS[17] = 100; 		//	KV Maximum
	SETTINGS[18] = 50; 			//	MA Minimum
	SETTINGS[19] = 100; 		//	MA Maximum
	SETTINGS[20] = 50; 			//	MS Minimum
	SETTINGS[21] = 100; 		//	MS Maximum
	
	SETTINGS[22] = 30; 			//	Pause
	SETTINGS[23] = 5; 			//	Prepare
}


void MX_FLASH_SaveSettings(void) {
	HAL_FLASH_Unlock(); 
	FLASH_Erase_Sector(FLASH_SECTOR_3, VOLTAGE_RANGE_3);
	for (uint8_t i=0; i<SETTINGS_SIZE; i++) HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, SETTINGS_MX_ADDRES+i*2, SETTINGS[i]);
	HAL_FLASH_Lock();
}


void MX_FLASH_LoadSettings(void) {
	MX_FLASH_DefaultSettings();
	if (*(uint32_t*)(SETTINGS_MX_ADDRES) == 0xFFFFFFFF) MX_FLASH_SaveSettings();
	else for (uint8_t i=0; i<SETTINGS_SIZE; i++) SETTINGS[i] = *(uint16_t*)(SETTINGS_MX_ADDRES+i*2);
}
