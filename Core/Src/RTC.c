/*
 * RTC.c
 *
 *  Created on: Mar 11, 2023
 *      Author: lees
 */

#include "main.h"
#include "cmsis_os.h"
void print_rtc(void)
{


	RTC_DateTypeDef rtc_date; // 날짜
	RTC_TimeTypeDef rtc_time; // 시간

	static char printtime[50]; // 시간 출력
	static char printdate[50]; // 날짜 출력

	//현재 포맷 얻기
	char *format;

	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN); // 시간 얻기
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN); // 날짜 얻기


	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

	sprintf(printtime, "%s : %02d:%02d:%02d %s","T",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format);
	sprintf(printdate, "%s : %02d-%02d-%02d","D", 2000 + rtc_date.Year,rtc_date.Month, rtc_date.Date);
	lcd_set_cursor(1, 1);
	osDelay(pdMS_TO_TICKS(2));
	lcd_string_write(printdate);
	osDelay(pdMS_TO_TICKS(20));
	lcd_set_cursor(2, 1);
	osDelay(pdMS_TO_TICKS(2));
	lcd_string_write(printtime);
	osDelay(pdMS_TO_TICKS(2));
}
