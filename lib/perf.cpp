#include <stdlib.h>
#include "perf.h"


perf::perf()
{
    pretv.tv_sec = 0;
    pretv.tv_usec = 0; 

    sumtv.tv_sec = 0;
    sumtv.tv_usec = 0; 
    
    status = false;
}

void perf::start()
{
    pretv.tv_sec = 0;
    pretv.tv_usec = 0; 

    sumtv.tv_sec = 0;
    sumtv.tv_usec = 0; 
    
    gettimeofday(&pretv, NULL);
    status = true;
}
void perf::stop()
{
    pause();
}

void perf::pause()
{
    long scosts;
	long uscosts;	
	struct timeval current_tv;

    if(status == false)
        return;
    
	gettimeofday(&current_tv,NULL);

	scosts = current_tv.tv_sec - pretv.tv_sec + sumtv.tv_sec - 1;
	uscosts = current_tv.tv_usec - pretv.tv_usec + sumtv.tv_usec + USECS_COUNT;
	
	if(uscosts >= USECS_COUNT)
	{
		scosts += (uscosts / USECS_COUNT);
		uscosts = uscosts % USECS_COUNT;
	}
	
	sumtv.tv_sec = scosts;
	sumtv.tv_usec = uscosts;

    status = false;
}

void perf::recovery()
{
    if(status == true)
        return;
    
    gettimeofday(&pretv, NULL);
    status = true;
}

double perf::gettimegap()
{
    long scosts = 0;
	long uscosts = 0;
    double timegap = 0;
	struct timeval current_tv;

    if(status == false)
    {
       timegap = sumtv.tv_sec;
       timegap *= USECS_COUNT;
       timegap += sumtv.tv_usec;
    }
    else
    {
    	gettimeofday(&current_tv,NULL);

    	scosts = current_tv.tv_sec - pretv.tv_sec + sumtv.tv_sec - 1;
    	uscosts = current_tv.tv_usec - pretv.tv_usec + sumtv.tv_usec + USECS_COUNT;
    	
    	if(uscosts >= USECS_COUNT)
    	{
    		scosts += (uscosts / USECS_COUNT);
    		uscosts = uscosts % USECS_COUNT;
    	}

        timegap = scosts;
        timegap *= USECS_COUNT;
        timegap += uscosts;
    }
    
    return timegap; 
    
}


