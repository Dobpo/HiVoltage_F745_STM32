#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

/* ------------------------------------------------------ */



/* ------------------------------------------------------ */

extern void tcp_server_reply(uint8_t *buf, uint16_t dlen);
extern uint16_t tcp_fill_data (uint8_t *buf, const uint8_t *s, uint16_t len);

/* ------------------------------------------------------ */

uint8_t MX_ENC26J60_Checksum(uint8_t *array, uint8_t len);
void MX_ENC26J60_COM_Unknown(void);
void MX_ENC26J60_COM_ChangeIP(void);
void MX_ENC26J60_COM_Power(void);
void MX_ENC26J60_COM_Triac(void);
void MX_ENC26J60_COM_Check(void);
void MX_ENC26J60_COM_Load(void);
void MX_ENC26J60_COM_Save(void);
void MX_ENC26J60_COM_Prepare(void);
void MX_ENC26J60_COM_Xray(void);
void MX_ENC26J60_COM_Cancel(void);

/* ------------------------------------------------------ */

#define DATA_START 				54
#define BUFFER_SIZE 			1100
extern uint8_t READ_BUFFER [BUFFER_SIZE];

/* ------------------------------------------------------ */

uint8_t MX_ENC26J60_Checksum(uint8_t *array, uint8_t len) {
	uint8_t tmpValue;
	tmpValue = array[0];
	for (uint16_t idx=2; idx<len; idx++) tmpValue = tmpValue ^ array[idx];
	tmpValue += 0x80; return tmpValue;
}


void MX_ENC26J60_COM_Unknown(void) {
}


void MX_ENC26J60_COM_ChangeIP(void) {
}


void MX_ENC26J60_COM_Power(void) {
	uint8_t WRITE_BUFFER[3];
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	WRITE_BUFFER[2] = READ_BUFFER[DATA_START+2]; 
	
	if (WRITE_BUFFER[2] == 1) { 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET); osDelay(20); 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);
	} else {		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET); osDelay(20); 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);
	}	
	
	WRITE_BUFFER[1] = MX_ENC26J60_Checksum(WRITE_BUFFER, sizeof(WRITE_BUFFER)); 
	tcp_server_reply(READ_BUFFER, tcp_fill_data(READ_BUFFER, WRITE_BUFFER, sizeof(WRITE_BUFFER)));
}


void MX_ENC26J60_COM_Triac(void) {
}


void MX_ENC26J60_COM_Check(void) {
}


void MX_ENC26J60_COM_Load(void) {
}


void MX_ENC26J60_COM_Save(void) {
}


void MX_ENC26J60_COM_Prepare(void) {
}


void MX_ENC26J60_COM_Xray(void) {
}


void MX_ENC26J60_COM_Cancel(void) {
}
