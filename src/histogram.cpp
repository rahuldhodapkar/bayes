#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"

/* ==== histogram.cpp ====
 * implementation for the histogram class datatype laid out in histogram.h
 */

// Histogram constructor
Histogram::Histogram(int buckets)
{
    nBuckets = buckets;
    values = new unsigned long[nBuckets];    
    bounds = new BucketBound[nBuckets];
}

// Histogram destructor
Histogram::~Histogram()
{
    delete[] values;
    delete[] bounds;
}



