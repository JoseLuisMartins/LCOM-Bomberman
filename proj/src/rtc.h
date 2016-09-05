#ifndef __RTC_H
#define __RTC_H


/** @defgroup rtc rtc
 * @{
 *
 * Functions for using rtc
 */


#include <minix/drivers.h>
#include "i8254.h"

#define RTC_ADDR_REG	0x70
#define RTC_DATA_REG	0x71

#define SEC_ADDR		    0
#define MIN_ADDR		    2
#define HOUR_ADDR		    4
#define DAY_WEEK_ADDR	    6
#define DAY_MONTH_ADDR	    7
#define MONTH_ADDR		    8
#define YEAR_ADDR		    9
#define RTC_REG_A      		10
#define RTC_REG_B      		11
#define RTC_REG_C      		12
#define RTC_REG_D      		13

#define RTC_BIN_INFO		BIT(2)
#define UIP_SET				BIT(7)


typedef enum {
	SUNDAY = 0,
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6
} DAY_OF_WEEK;



typedef struct {
	unsigned long week_day;
	unsigned long month_day;
	unsigned long month;
	unsigned long year;
} date_info_t;


typedef struct {
	unsigned long seconds;
	unsigned long minutes;
	unsigned long hours;
} time_info_t;



/**
 * @brief Enables all interrupts
 */
void rtc_enable_int();



/**
 * @brief Disables all interrupts
 */
void rtc_disable_int();



/**
 * @brief Waits for valid data to be read
 */
void wait_valid_rtc();



/**
 * @brief Retrieves time of the day info
 *
 * @param[out] time	pointer to current day's time info
 *
 * @return Returns 0 on success, 1 otherwise
 */
int getCurrentTime(time_info_t *time);



/**
 * @brief Retrieves date info
 *
 * @param[out] date	pointer to current date's info
 *
 * @return Returns 0 on success, 1 otherwise
 */
int getCurrentDate(date_info_t *date);



/**
 * @brief Converts number on BCD to Binary
 *
 * @param num number on BCD notation
 *
 * @return Number on binary notation
 */
unsigned long BCD_TO_BINARY(unsigned long num);


/**@} */

#endif
