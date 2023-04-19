/*
RTC.h
    The following code was written by Uranium-239 with debugging help from Napalm. The
    author, Uraninium-239, requests that you leave in these two credits.
 
    Please feel free to modify this source.
*/
 
#ifndef _RTC_H_
#define _RTC_H_
 
typedef struct {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day_of_week;
    unsigned char day_of_month;
    unsigned char month;
    unsigned char year;
} time_t;
 
time_t get_global_time();
const char *get_day_name();
const char *get_month_name();
extern void gettime(time_t* time);
extern void rtc_install(void);
 
#endif