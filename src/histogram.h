#ifndef HISTOGRAM_H             // HEADER GUARD
#define HISTOGRAM_H 1

#include <iostream> 
#include <utility>

#include "run.h"

/* ==== histogram.h ====
 * class prototype for a histogram.  Contains information about the 
 * buckets in a vector of unsigned longs, and contains information about
 * the ranges in a vector of int pairs.
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 6.12.14
 */

typedef std::pair<double,double> Bounds;

class RangeSummary{
    public:
        double low;
        double high;
        double nReturned;
};

// define Histogram class wrapper
class Histogram 
{
    static bool rangeBoundOrderingFunction(const Run&, const Run&);
    static bool splitOrderingFunction(const Run&, const Run&);
    public:
        int nBuckets;
        int nObs;                   // number of observations seen
        double* values;
        Bounds* bounds;

        void update(RangeSummary&);
        void restructure();
        double getFreqOnRange(Bounds&);
        Histogram(int size, double binInit, double lowBound, double highBound);
        ~Histogram();
};

// histogram pretty print overloading
std::ostream& operator<<(std::ostream &strm, const Histogram &hist);

#endif
