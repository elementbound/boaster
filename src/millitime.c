#include <time.h>
#include "include/millitime.h"

double get_millitime() {
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);

    return tp.tv_sec * 1000.0 + tp.tv_nsec / 1000000.0;
}
