#ifndef HISTOGRAM_H             // HEADER GUARD
#define HISTOGRAM_H 1

#include <iostream> 

/* ==== histogram.h ====
 * class prototype for a histogram.  Contains information about the 
 * buckets in a vector of unsigned longs, and contains information about
 * the ranges in a vector of int pairs.
 */

// define BucketBound structure
typedef struct {
    int start;
    int end;
} BucketBound;

// define Histogram class wrapper
class Histogram 
{ 
    public:
        int nBuckets;
        unsigned long* values;
        BucketBound* bounds;

        Histogram(int size=10);
        ~Histogram();
};

// histogram pretty print overloading
std::ostream& operator<<(std::ostream &strm, const Histogram &hist);

#endif
