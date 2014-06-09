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
    values = new double[nBuckets];    
    bounds = new BucketBound[nBuckets];
}

// Histogram destructor
Histogram::~Histogram()
{
    delete[] values;
    delete[] bounds;
}

std::ostream& operator<<(std::ostream &strm, const Histogram &hist) {
    std::ostream& retStrm = strm;
    for(int i = 0; i < hist.nBuckets; i++) {
        retStrm << hist.bounds[i].low << "\t" 
                << hist.bounds[i].high << "\t"
                << hist.values[i] << std::endl;
    }
    return retStrm;
}
