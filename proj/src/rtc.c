/** @file */
#include "rtc.h"


void rtc_enable_int() {
	asm("sti");
}


void rtc_disable_int() {
	asm("cli");
}


void wait_valid_rtc() {
	unsigned long regA = 0;
	do {
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &regA);
	} while ( regA & UIP_SET);
}


int getCurrentTime(time_info_t *time) {
	unsigned long data;

	wait_valid_rtc();
	unsigned long seconds;
	if(sys_outb(RTC_ADDR_REG, SEC_ADDR))
		return 1;
	if(sys_inb(RTC_DATA_REG, &seconds))
		return 1;


	wait_valid_rtc();
	unsigned long minutes;
	if(sys_outb(RTC_ADDR_REG, MIN_ADDR))
		return 1;
	if(sys_inb(RTC_DATA_REG, &minutes))
		return 1;


	wait_valid_rtc();
	unsigned long hours;
	if (sys_outb(RTC_ADDR_REG, HOUR_ADDR))
		return 1;
	if (sys_inb(RTC_DATA_REG, &hours))
		return 1;


	if(sys_outb(RTC_ADDR_REG, RTC_REG_B))
		return 1;
	if(sys_inb(RTC_DATA_REG, &data))
		return 1;


	if( !(data & RTC_BIN_INFO) )
	{
		seconds = BCD_TO_BINARY(seconds);
		minutes = BCD_TO_BINARY(minutes);
		hours = BCD_TO_BINARY(hours);
	}

	time->seconds = seconds;
	time->minutes = minutes;
	time->hours = hours;
	return 0;
}



int getCurrentDate(date_info_t *date) {
	unsigned long data;

	unsigned long week_day;
	if(sys_outb(RTC_ADDR_REG, DAY_WEEK_ADDR))
		return 1;
	if(sys_inb(RTC_DATA_REG, &week_day))
		return 1;

	unsigned long month_day;
	if(sys_outb(RTC_ADDR_REG, DAY_MONTH_ADDR))
		return 1;
	if(sys_inb(RTC_DATA_REG, &month_day))
		return 1;


	unsigned long month;
	if(sys_outb(RTC_ADDR_REG, MONTH_ADDR))
		return 1;
	if(sys_inb(RTC_DATA_REG, &month))
		return 1;


	unsigned long year;
	if(sys_outb(RTC_ADDR_REG, YEAR_ADDR))
		return 1;
	if(sys_inb(RTC_DATA_REG, &year))
		return 1;


	if(sys_outb(RTC_ADDR_REG, RTC_REG_B))
		return 1;
	if(sys_inb(RTC_DATA_REG, &data))
		return 1;

	if( !(data & RTC_BIN_INFO) )
	{
		week_day = BCD_TO_BINARY(week_day);
		month_day = BCD_TO_BINARY(month_day);
		month = BCD_TO_BINARY(month);
		year = BCD_TO_BINARY(year);
	}

	date->week_day = week_day;
	date->month_day = month_day;
	date->month = month;
	date->year = year;
	return 0;
}





unsigned long BCD_TO_BINARY(unsigned long num){
	return ((num >> 4) * 10) + (num & 0x0F);
}
