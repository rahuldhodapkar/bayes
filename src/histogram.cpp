#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"
using namespace std;

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

std::ostream& operator<<(std::ostream &strm, const Histogram &hist) {
    ostream& retStrm = strm;
    for(int i = 0; i < hist.nBuckets; i++) {
        retStrm << hist.bounds[i].start << "\t" 
                << hist.bounds[i].end << "\t"
                << hist.values[i] << endl;
    }
    return retStrm;
}
