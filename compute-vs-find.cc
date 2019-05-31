
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#include <Memoization_uthash.h>

/*
 * Find
 */

MemoSupport *tester ;

double* findConsumption ( float utilization )
{
    return tester->find(sizeof(float), &utilization) ;
}


/*
 * Compute
 */

double staticPower  = 10 ;
double constant     = 10 ;
double timeInterval = 10 ;

double consumption ;

// CUBIC
double* calculateConsumption_CUBIC ( float utilization )
{
    double* consumption = new double();
    *consumption = (staticPower + constant * pow(utilization,3)) * timeInterval;
    return consumption;
}

// Linear
double* calculateConsumption_LINEAR ( float utilization )
{
    double* consumption = new double();
    *consumption = (staticPower + constant * utilization) * timeInterval;
    return consumption;
}

// Square
double* calculateConsumption_SQUARE ( float utilization )
{
    double* consumption = new double();
    *consumption = (staticPower + constant * pow(utilization,2)) * timeInterval;
    return consumption;
}

// SQRT
double* calculateConsumption_SQRT ( float utilization )
{
    double* consumption = new double();
    *consumption = (staticPower + constant * sqrt(utilization)) * timeInterval;
    return consumption;
}


/*
 * Tester
 */

#define MSG(x) #x
#define MSG2(x) MSG(x)

// http://www.binarytides.com/get-time-difference-in-microtime-in-c/
double time_diff ( struct timeval x , struct timeval y )
{
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}


int main ( int argc, char *argv[] )
{
    int i ;
    double *ret ;
    struct timeval before , after;

    // header
    printf("Compiled; Algorithm; Time-elapsed (us)\n"); 

    // data
    gettimeofday(&before , NULL);
    for (i=0; i<1000; i++) {
         ret = calculateConsumption_CUBIC((float)i) ;
    }
    gettimeofday(&after , NULL);
    printf("%s; CUBIC; %.0lf\n" , MSG2(OPT), time_diff(before , after) ); 

    gettimeofday(&before , NULL);
    for (i=0; i<1000; i++) {
         ret = calculateConsumption_LINEAR((float)i) ;
    }
    gettimeofday(&after , NULL);
    printf("%s; LINEAR; %.0lf\n" , MSG2(OPT), time_diff(before , after) ); 

    gettimeofday(&before , NULL);
    for (i=0; i<1000; i++) {
         ret = calculateConsumption_SQUARE((float)i) ;
    }
    gettimeofday(&after , NULL);
    printf("%s; SQUARE; %.0lf\n" , MSG2(OPT), time_diff(before , after) ); 

    gettimeofday(&before , NULL);
    for (i=0; i<1000; i++) {
         ret = calculateConsumption_SQRT((float)i) ;
    }
    gettimeofday(&after , NULL);
    printf("%s; SQRT; %.0lf\n" , MSG2(OPT), time_diff(before , after) ); 

    // data: find
    tester = new MemoSupport("testing") ;
    for (i=0; i<1000; i++) {
         ret = calculateConsumption_SQRT((float)i) ;
         tester->add(sizeof(double), (float *)&i, *ret) ;
    }

    gettimeofday(&before , NULL);
    for (i=0; i<1000; i++) {
         ret = findConsumption((float)i) ;
    }
    gettimeofday(&after , NULL);
    printf("%s; FIND; %.0lf\n" , MSG2(OPT), time_diff(before , after) ); 

    // end
    return 0;
}

