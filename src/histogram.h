#ifndef HISTOGRAM_H             // HEADER GUARD
#define HISTOGRAM_H

#include <iostream> 

/* ==== histogram.h ====
 * class prototype for a histogram.  Contains information about the 
 * buckets in a vector of unsigned longs, and contains information about
 * the ranges in a vector of int pairs.
 */

class Histogram 
{ 
    typedef struct {
        int start;
        int end;
    } BucketBound;
    unsigned long* values;
    BucketBound* bounds;
}











#endif
