/*
 * i2c.c
 *
 *  Created on: 2023. 3. 7.
 *      Author: lees
 */
#include "main.h"
#include "cmsis_os.h"


void I2C_LCD_Init(void)
{
	HAL_Delay(40); // 1. 켜지고 40ms이상 대기
	I2C_LCD_4bit_cmd_WRITE(0x30);
	HAL_Delay(5);	// 2. 4.1ms 이상 대기
	I2C_LCD_4bit_cmd_WRITE(0x30);
	delay_us(150); // 3. 100us 대기
	I2C_LCD_4bit_cmd_WRITE(0x30);
	I2C_LCD_4bit_cmd_WRITE(0x20);
	I2C_LCD_4bit_cmd_WRITE(0x20);
	I2C_LCD_4bit_cmd_WRITE(0x80);
	I2C_LCD_4bit_cmd_WRITE(0x00);
	I2C_LCD_4bit_cmd_WRITE(0xE0);
	lcd_display_clear();
	I2C_LCD_4bit_cmd_WRITE(0x00);
	I2C_LCD_4bit_cmd_WRITE(0x60);

}

void lcd_display_clear(void)
{
	//디스플레이 클리어 커맨드 전송
	I2C_LCD_4bit_cmd_WRITE(0x00);
	I2C_LCD_4bit_cmd_WRITE(0x10);
	//2ms의 딜레이 줘야 함
	//HAL_Delay(2);
	osDelay(pdMS_TO_TICKS(2));
}


//디스플레이의 위치를 홈으로 옮기기
void lcd_display_return_home(void)
{
	I2C_LCD_4bit_cmd_WRITE(0x00);
	I2C_LCD_4bit_cmd_WRITE(0x20);
	//2ms의 딜레이 줘야 함
//	HAL_Delay(2);
	osDelay(pdMS_TO_TICKS(2));
}


void lcd_set_cursor(uint8_t row, uint8_t column)
{
	column--;
	switch(row)
	{
		case 1: //1행인 경우
			//커서를 첫번째 행에 설정하고 인덱스를 추가
			//0b10000000의 데이터를 설정하여 1행의 첫밴째 열(0x00)로 커서를 설정
			lcd_cmd_write((column |= 0x80));
			break;
		case 2: //2행인 경우
			//커서를 두번째 행에 설정하고 인덱스를 추가
			//0b11000000의 데이터를 설정하여 2행의 첫밴째 열(0x40)로 커서를 설정
			lcd_cmd_write((column |= 0xC0));
			break;
		default:
			break;
	}
	//osDelay(pdMS_TO_TICKS(2));
}


void lcd_cmd_write(uint8_t cmd)
{
	I2C_LCD_4bit_cmd_WRITE(cmd & 0xF0); // 상위 4비트
	I2C_LCD_4bit_cmd_WRITE((cmd << 4) & 0xF0); // 하위 4비트
	//HAL_Delay(2);
	osDelay(pdMS_TO_TICKS(2));
}

void lcd_string_write(char *str)
{
	for(int i=0 ; str[i];i++)
	{
		lcd_data_write(str[i]);
	}
}
void lcd_data_write(uint8_t cmd)
{
	I2C_LCD_4bit_data_WRITE((char)cmd & 0xF0); // 상위 4비트
	I2C_LCD_4bit_data_WRITE(((char)cmd << 4) & 0xF0); // 하위 4비트
	//HAL_Delay(2);
	osDelay(pdMS_TO_TICKS(2));

}

void I2C_LCD_4bit_cmd_WRITE(uint8_t cmd)
{
	uint8_t cmd_s;
	uint8_t cmd_L[2];

	cmd_s = cmd & 0xF0; // 상위 4비트

	cmd_L[0] = cmd_s | 0x0C; //En = 1
	cmd_L[1] = cmd_s | 0x08; //En = 0
	//데이터 전송 시 중간에 En 핀을 High->Low로 설정하여 데이터 전송활성화

	if(HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, cmd_L, sizeof(cmd_L), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
}


void I2C_LCD_4bit_data_WRITE(uint8_t data)
{
	uint8_t data_u;
	uint8_t data_W[2];

	data_u = data & 0xF0; // 상위 4비트

	//데이터를 전송하므로 rs = 1이여야 함
	data_W[0] = data_u | 0b00001101; //En = 1
	data_W[1] = data_u | 0b00001001; //En = 0
	//데이터 전송 시 중간에 En 핀을 High->Low로 설정하여 데이터 전송활성화

	if(HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_W, sizeof(data_W), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
}
