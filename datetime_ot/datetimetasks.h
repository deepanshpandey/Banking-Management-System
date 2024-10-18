//
// Created by deepanshpandey on 10/17/24.
//

#ifndef DATETIMETASKS_H
#define DATETIMETASKS_H

#include <time.h>
#include "../db/db.h"
#include <stdio.h>

void initializeDateTime(DateTime *dt) {
    // Get the current time
    time_t now = time(NULL);

    // Convert it to local time
    struct tm *local = localtime(&now);

    // Populate the DateTime struct with current date and time
    dt->day = local->tm_mday;
    dt->month = local->tm_mon + 1;  // tm_mon is months since January (0-11), so add 1
    dt->year = local->tm_year + 1900;  // tm_year is years since 1900, so add 1900

    dt->hour = local->tm_hour;
    dt->minute = local->tm_min;
    dt->second = local->tm_sec;
}

void getDateTime(DateTime dt, const char *message) {
    sprintf(message,"%02d/%02d/%d %02d:%02d:%02d\n", dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second);
}
#endif //DATETIMETASKS_H
