#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

/* ------------------------------------------------------ */

extern DAC_HandleTypeDef hdac;

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;

/* ------------------------------------------------------ */

extern void MX_FLASH_DefaultIP(void);
extern void MX_FLASH_SaveIP(void);
extern void MX_FLASH_LoadIP(void);
extern void MX_FLASH_DefaultSettings(void);
extern void MX_FLASH_SaveSettings(void);
extern void MX_FLASH_LoadSettings(void);

extern void MX_ENC28J60_Init(void);
extern void tcp_server_reply(uint8_t *buf, uint16_t dlen);
extern uint16_t tcp_fill_data (uint8_t *buf, const uint8_t *s, uint16_t len);
extern uint16_t enc28j60PacketReceive (uint16_t maxlen, uint8_t* packet);
extern uint16_t packetloop_icmp_tcp(uint8_t *buf, uint16_t plen);

/* ------------------------------------------------------ */

void MX_MAIN_StartupInit(void);
void MX_MAIN_StartupError(uint8_t errCode);
void MX_MAIN_CalculateValues(void);
void MX_MAIN_CheckPackets(void);

uint8_t MX_ENC26J60_Checksum(uint8_t *array, uint16_t len, uint8_t start);
void MX_ENC26J60_SendReply(uint16_t len);
void MX_ENC26J60_COM_Error(void);
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

void MAIN_FatalError(void);
/* ------------------------------------------------------ */

#define SETTINGS_SIZE 		24
uint16_t SETTINGS [SETTINGS_SIZE];

#define DATA_START 				54
#define BUFFER_SIZE 			1500
uint8_t READ_BUFFER [BUFFER_SIZE];
uint8_t WRITE_BUFFER [BUFFER_SIZE];

#define VOLTAGE_MULTI 		10
#define VOLTAGE_SIZE 			7
uint16_t ADC1_TMP_VALUES [VOLTAGE_SIZE];
uint16_t ADC1_DMA_VALUES [VOLTAGE_SIZE];
uint16_t ADC1_DMA_BUFFER [VOLTAGE_SIZE*VOLTAGE_MULTI];

#define CURRENT_SIZE 			14000
#define MEASURE_SIZE 			56000
uint16_t ADC2_DMA_BUFFER [CURRENT_SIZE];
uint16_t ADC3_DMA_BUFFER [MEASURE_SIZE];

uint32_t MODE_COUNT, MODE_PULSE;
uint16_t MODE_PERIOD, MODE_NAKAL, MODE_KV;
/* ------------------------------------------------------ */
//0 - Puskatel
//1 - Semister
uint8_t ExpectedStatus[2];
uint8_t ValidStatus[2];
/* ------------------------------------------------------ */

extern uint8_t ipaddr[4];

/* ------------------------------------------------------ */

void MX_MAIN_StartupInit(void)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET); osDelay(1);
	if (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)) MX_MAIN_StartupError(1);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET); osDelay(1);
	if (!HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)) MX_MAIN_StartupError(2);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET); osDelay(333);

  TIM_CCxChannelCmd(htim11.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE); __HAL_TIM_MOE_ENABLE(&htim11);
	TIM11->ARR = 0xFFFF; TIM11->CCR1 = 0xFFFF; __HAL_TIM_ENABLE(&htim11); osDelay(1);
	if (!HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)) MX_MAIN_StartupError(3); __HAL_TIM_DISABLE(&htim11);
	TIM11->ARR = 0xFFFF; TIM11->CCR1 = 0x0000; __HAL_TIM_ENABLE(&htim11); osDelay(1);
	if (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)) MX_MAIN_StartupError(4); __HAL_TIM_DISABLE(&htim11);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET); osDelay(333);
	
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,0); HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,0); HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET); osDelay(333);
		
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1_DMA_BUFFER, sizeof(ADC1_DMA_BUFFER)); 	
	__HAL_DMA_DISABLE_IT(&hdma_adc1,DMA_IT_HT); __HAL_DMA_DISABLE_IT(&hdma_adc1,DMA_IT_TC);	
	__HAL_DMA_DISABLE_IT(&hdma_adc1,DMA_IT_TE); __HAL_DMA_DISABLE_IT(&hdma_adc1,DMA_IT_DME); __HAL_DMA_DISABLE_IT(&hdma_adc1,DMA_IT_FE);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2_DMA_BUFFER, sizeof(ADC2_DMA_BUFFER)); 
	__HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_HT); __HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_TC);	
	__HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_TE); __HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_DME); __HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_FE);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)ADC3_DMA_BUFFER, sizeof(ADC3_DMA_BUFFER)); 	
	__HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_HT); __HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_TC);	
	__HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_TE); __HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_DME); __HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_FE);
		
	MX_FLASH_LoadSettings(); MX_FLASH_LoadIP(); MX_ENC28J60_Init(); osDelay(333);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET); osDelay(20); 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
}


void MX_MAIN_StartupError(uint8_t errCode) {
	__HAL_TIM_DISABLE(&htim11);
	while (1) {
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
		osDelay(250);
	}
}


void MX_MAIN_CalculateValues(void) {
	for (uint8_t idx=0; idx<VOLTAGE_SIZE; idx++) {
		ADC1_TMP_VALUES[idx] = 0;
		for (uint8_t cnt=0; cnt<VOLTAGE_MULTI; cnt++) ADC1_TMP_VALUES[idx] += ADC1_DMA_BUFFER[VOLTAGE_SIZE*cnt+idx];
		ADC1_DMA_VALUES[idx] = ADC1_TMP_VALUES[idx] / VOLTAGE_MULTI;
	} 
}


void MX_MAIN_CheckPackets(void) {
	uint16_t PACKLEN;
	PACKLEN = enc28j60PacketReceive(BUFFER_SIZE, READ_BUFFER);
	if (PACKLEN == 0) return;
	PACKLEN = packetloop_icmp_tcp(READ_BUFFER, PACKLEN);
	if (PACKLEN == 0) return;
	if (READ_BUFFER[DATA_START+1] == MX_ENC26J60_Checksum(READ_BUFFER, PACKLEN, DATA_START)){
		switch (READ_BUFFER[DATA_START]){
			case 0x0F: MX_ENC26J60_COM_ChangeIP(); break;
			case 0x1F: MX_ENC26J60_COM_Power(); break;
			case 0x2F: MX_ENC26J60_COM_Triac(); break;
			case 0x3F: MX_ENC26J60_COM_Check(); break;
			case 0x4F: MX_ENC26J60_COM_Load(); break;
			case 0x5F: MX_ENC26J60_COM_Save(); break;
			case 0x6F: MX_ENC26J60_COM_Prepare(); break;
			case 0x7F: MX_ENC26J60_COM_Xray(); break;
			case 0x8F: MX_ENC26J60_COM_Cancel(); break;
			default: MX_ENC26J60_COM_Unknown(); break;
		}
	} else MX_ENC26J60_COM_Error();
}


uint8_t MX_ENC26J60_Checksum(uint8_t *array, uint16_t len, uint8_t start) {
	uint8_t tmpValue = array[start];
	for (uint16_t idx=start+2; idx<start+len; idx++) tmpValue = tmpValue ^ array[idx];
	tmpValue += 0x80; return tmpValue;
}


void MX_ENC26J60_SendReply(uint16_t len) {
	WRITE_BUFFER[1] = MX_ENC26J60_Checksum(WRITE_BUFFER, len, 0); 
	tcp_server_reply(READ_BUFFER, tcp_fill_data(READ_BUFFER, WRITE_BUFFER, len));
}


void MX_ENC26J60_COM_Error(void) {
}


void MX_ENC26J60_COM_Unknown(void) {
}


void MX_ENC26J60_COM_ChangeIP(void) {
	uint8_t WRITE_BUFFER[6];
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	WRITE_BUFFER[2] = ipaddr[0]; WRITE_BUFFER[3] = ipaddr[1];
	WRITE_BUFFER[4] = ipaddr[2]; WRITE_BUFFER[5] = ipaddr[3];
	WRITE_BUFFER[1] = MX_ENC26J60_Checksum(WRITE_BUFFER, sizeof(WRITE_BUFFER),0);
	tcp_server_reply(READ_BUFFER, tcp_fill_data(READ_BUFFER, WRITE_BUFFER, sizeof(WRITE_BUFFER)));
}


void MX_ENC26J60_COM_Power(void) {
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	WRITE_BUFFER[2] = READ_BUFFER[DATA_START+2]; 
	
	if (WRITE_BUFFER[2] == 0x01) { 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET); osDelay(20); 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);
	} else {		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET); osDelay(20); 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);
	}	MX_ENC26J60_SendReply(3);
	
	//Время заряда?
}


void MX_ENC26J60_COM_Triac(void) {
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	WRITE_BUFFER[2] = READ_BUFFER[DATA_START+2]; 

	if (WRITE_BUFFER[2] == 0x01) {
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET); 
	} else { 
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET); 
	}	MX_ENC26J60_SendReply(3);
}


void MX_ENC26J60_COM_Check(void) {
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	
	for (uint8_t i=0;i<VOLTAGE_SIZE;i++) {
		WRITE_BUFFER[i*2+2] = ADC1_DMA_VALUES[i] >> 8; 
		WRITE_BUFFER[i*2+3] = ADC1_DMA_VALUES[i] & 0xFF;
	}	MX_ENC26J60_SendReply(16);
}


void MX_ENC26J60_COM_Load(void) {
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	
	for (uint8_t i=0;i<SETTINGS_SIZE;i++) {	
		WRITE_BUFFER[i*2+2] = SETTINGS[i] >> 8;	
		WRITE_BUFFER[i*2+3] = SETTINGS[i] & 0xFF; 
	}	MX_ENC26J60_SendReply(50);
}


void MX_ENC26J60_COM_Save(void) {
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	WRITE_BUFFER[2] = 0xFF; 
	
	for (uint8_t i=0;i<SETTINGS_SIZE;i++){
		SETTINGS[i] = (READ_BUFFER[DATA_START+i*2+2] << 8) + READ_BUFFER[DATA_START+i*2+3];
	}	MX_FLASH_SaveSettings();
	MX_ENC26J60_SendReply(3);
}


void MX_ENC26J60_COM_Prepare(void) {
	uint8_t WRITE_BUFFER[3];
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	WRITE_BUFFER[2] = 0xFF; 
	
	MODE_PULSE = SETTINGS[14] * 216 / 1000;
	MODE_PERIOD = SETTINGS[15] * 216 / 1000;
	
	MODE_NAKAL = (READ_BUFFER[DATA_START+2] << 8) + READ_BUFFER[DATA_START+3];
	
	MODE_KV = (READ_BUFFER[DATA_START+4] << 8) + READ_BUFFER[DATA_START+5];
	
	MODE_COUNT = ((READ_BUFFER[DATA_START+6] << 8) + READ_BUFFER[DATA_START+7]) * 1000000 / SETTINGS[16];//16 - ?
	
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2_DMA_BUFFER, sizeof(ADC2_DMA_BUFFER)/2); //__HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_TC);	
	__HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_HT); __HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_TE); 
	__HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_DME); __HAL_DMA_DISABLE_IT(&hdma_adc2,DMA_IT_FE);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)ADC3_DMA_BUFFER, sizeof(ADC3_DMA_BUFFER)/2); //__HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_TC);		
	__HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_HT); __HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_TE); 
	__HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_DME); __HAL_DMA_DISABLE_IT(&hdma_adc3,DMA_IT_FE);
	
	WRITE_BUFFER[1] = MX_ENC26J60_Checksum(WRITE_BUFFER, sizeof(WRITE_BUFFER),0); 
	tcp_server_reply(READ_BUFFER, tcp_fill_data(READ_BUFFER, WRITE_BUFFER, sizeof(WRITE_BUFFER)));
}


void MX_ENC26J60_COM_Xray(void) {
	uint8_t WRITE_BUFFER[3];
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START];
	WRITE_BUFFER[2] = 0xFF; 	
	
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R, MODE_KV*21.39165); 
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
	TIM11->ARR = MODE_PERIOD; TIM11->CCR1 = MODE_PULSE; TIM11->CNT = 0;
	__HAL_TIM_ENABLE_IT(&htim11, TIM_IT_CC1); __HAL_TIM_ENABLE(&htim11);
	
	__HAL_TIM_ENABLE(&htim8); 
	__HAL_TIM_ENABLE(&htim6);
	
	WRITE_BUFFER[1] = MX_ENC26J60_Checksum(WRITE_BUFFER, sizeof(WRITE_BUFFER),1); 
	tcp_server_reply(READ_BUFFER, tcp_fill_data(READ_BUFFER, WRITE_BUFFER, sizeof(WRITE_BUFFER)));
}


void MX_ENC26J60_COM_Cancel(void) {
	uint8_t WRITE_BUFFER[3];
	WRITE_BUFFER[0] = READ_BUFFER[DATA_START]; 
	WRITE_BUFFER[2] = 0xFF; 

	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,0); HAL_DAC_Start(&hdac,DAC_CHANNEL_1);	
		
	WRITE_BUFFER[1] = MX_ENC26J60_Checksum(WRITE_BUFFER, sizeof(WRITE_BUFFER),0); 
	tcp_server_reply(READ_BUFFER, tcp_fill_data(READ_BUFFER, WRITE_BUFFER, sizeof(WRITE_BUFFER)));
}

void MAIN_FatalError(void){
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET); osDelay(20); 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	
	WRITE_BUFFER[0] = 0x88; WRITE_BUFFER[1] = 0x88; WRITE_BUFFER[2] = 0x88;
	MX_ENC26J60_SendReply(3);
}
