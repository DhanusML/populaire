#include <sys/time.h>
#include "utils.h"


int
start_measurement(Stats *stats)
{
    int ret_val = 0;
    struct timeval point;
    gettimeofday(&point, NULL);
    stats->start_t = (double) point.tv_sec;
    return ret_val;
}


int
end_measurement(Stats *stats)
{
    int ret_val =0;
    struct timeval point;
    gettimeofday(&point, NULL);
    stats->end_t = (double)point.tv_sec;
    return ret_val;
}


int
compute_stats(Stats *stats)
{
    int ret_val = 0;
    double elapsed = stats->end_t - stats->start_t;
    stats->cps_raw = (double)stats->raw_ch / elapsed;
    stats->wpm_raw = stats->cps_raw * 12; // *60/5
    stats->cps = (double) stats->ch / elapsed;
    stats->wpm = stats->cps * 12;
    stats->err = (double)stats->raw_ch / stats->ch - 1;
    return ret_val;
}
