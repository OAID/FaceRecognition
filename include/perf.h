#ifndef PERF_H__
#define PERF_H__


#include <sys/time.h>

#define USECS_COUNT    1000000

class perf
{
public:
    perf();

    void start();
    void stop();
    // pause to counte time
    void pause();
    // recovery to counte time
    void recovery();

    // get the timeinterval(us), from start to now, 
    // exclude the interval from pause to recovery
    double gettimegap();
    
private:
    struct timeval pretv;
    struct timeval sumtv;
    bool   status;
};

#endif

