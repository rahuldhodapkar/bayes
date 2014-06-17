#include <stdlib.h>
#include <stdio.h>
#include <utility>
#include "histogram.h"

/* ==== histogram.cpp ====
 * implementation for the histogram class datatype laid out in histogram.h
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 6.12.14
 */

// Histogram constructor
Histogram::Histogram(int buckets)
{
    nBuckets = buckets;
    values = new double[nBuckets];    
    bounds = new BucketBound[nBuckets];
    nObs = 0;
}

// Histogram destructor
Histogram::~Histogram()
{
    delete[] values;
    delete[] bounds;
}

double Histogram::getFreqOnRange(Bounds& range)
{
    double freq = 0;
    for (int i = 0; i < nBuckets; i++) {
        double overlap = std::min(range.second, bounds[i].high) -
                         std::max(range.first, bounds[i].low);
        overlap = std::max(overlap / (bounds[i].high-bounds[i].low), 0.0);
        freq += overlap * values[i];
    }
    return freq;
}

std::ostream& operator<<(std::ostream &strm, const Histogram &hist) {
    std::ostream& retStrm = strm;
    for(int i = 0; i < hist.nBuckets; i++) {
        retStrm << hist.bounds[i].low << "," 
                << hist.bounds[i].high << ","
                << hist.values[i] << std::endl;
    }
    return retStrm;
}
