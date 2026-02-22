/*
 * File: ProfileStats
 * Author: Michael Franks 
 * Description: a block of data for capturing application data
 */

#ifndef PROFILE_STATS_H
#define PROFILE_STATS_H

struct stats
{
    double uptime_sec = 0.0;
    double frame_ms = 0.0;

    int thread_count = 0;
};

#endif // PROFILE_STATS_H
