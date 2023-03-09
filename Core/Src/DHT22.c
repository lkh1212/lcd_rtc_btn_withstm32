/*
 * DHT22.c
 *
 *  Created on: Mar 9, 2023
 *      Author: lees
 */

#include "main.h"

void getTemp(void)
{
	GPIO_InitTypeDef tempgpio;

	uint8_t in_num[40] = {0};
	uint32_t bit1 = 0;
	uint32_t bit2 = 0;


	//1. 포트에서 1~10ms 동안 LOW 를 출력
	tempgpio.Pin = GPIO_PIN_2;
	tempgpio.Mode = GPIO_MODE_OUTPUT_PP;
	tempgpio.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &tempgpio);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_Delay(3); // 1~10ms 지연
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	//2.40us동안 딜레이 (HIGH상태)

	delay_us(35);
	//3.핀을 입력으로 바꾸고 80us동안 LOW 입력
	tempgpio.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOC, &tempgpio);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) != GPIO_PIN_SET);

	//4.80us 동안 HIGH 입력
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) != GPIO_PIN_RESET);

	//5.40비트 수신
	for(uint32_t i=0; i<40; i++)
	{
		bit1 = 0;
		bit2 = 0;
		__HAL_TIM_SET_COUNTER(&htim10,0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) != GPIO_PIN_SET);
		bit1 = __HAL_TIM_GET_COUNTER(&htim10);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) != GPIO_PIN_RESET);
		bit2 = __HAL_TIM_GET_COUNTER(&htim10);

		if (bit2 - bit1 > 30)
		{
			in_num[i] = 1;
		}else
		{
			in_num[i] = 0;
		}

	}

	uint16_t temp = 0;
	uint16_t humi = 0;
	for (int32_t i=31; i>=0; i--)
	{
		if (i > 15)
		{
			if(in_num[i] == 1)
			{
				//제곱은 다른 기호
				//humi += 1*(2^(31-i));
				temp += 1 * pow(2, 31-i);
			}
		}
		else
		{
			if(in_num[i] == 1)
			{
				humi += 1 * pow(2, 15-i);
			}
		}
	}
	temp /= 10;
	humi /= 10;
	char temp_str [5] ;
	char humi_str [5] ;
	sprintf(temp_str,"%d",temp);
	sprintf(humi_str,"%d",humi);
	lcd_set_cursor(1, 0);

	lcd_string_write("temp = ");
	lcd_string_write(temp_str);
}
