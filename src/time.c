/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <stdint.h>
#include <time.h>

#define SECONDS_IN_MINUTE 60
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24

#define SECONDS_IN_DAY 24 * 60 * 60

#define DAYS_IN_WEEK 7

#define DAYS_IN_YEAR_NONLEAP 365
#define DAYS_IN_YEAR_LEAP 366


struct tm *gmtime_r(const time_t *timep, struct tm *r) {
    // 将 time_t 转换为 struct tm 结构
    time_t t = *timep;
    static const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    r->tm_sec = t % SECONDS_IN_MINUTE;
    t /= SECONDS_IN_MINUTE;
    r->tm_min = t % MINUTES_IN_HOUR;
    t /= MINUTES_IN_HOUR;
    r->tm_hour = t % HOURS_IN_DAY;
    t /= HOURS_IN_DAY;

    r->tm_wday = (t + 4) % 7;
    if (r->tm_wday < 0) r->tm_wday += 7;

    int years = 1970;
    while (1) {
        int days = (years % 4 == 0 && (years % 100 != 0 || years % 400 == 0)) ? 366 : 365;
        if (t < days) break;
        t -= days;
        years++;
    }

    r->tm_year = years - 1900;
    r->tm_yday = t;

    for (int i = 0; i < 12; i++) {
        if (i == 1 && (years % 4 == 0 && (years % 100 != 0 || years % 400 == 0))) days_in_month[1] = 29;
        if (t < days_in_month[i]) {
            r->tm_mon = i;
            break;
        }
        t -= days_in_month[i];
    }

    r->tm_mday = t + 1;
    r->tm_isdst = 0; // UTC 时间不考虑夏令时

    return r;
}