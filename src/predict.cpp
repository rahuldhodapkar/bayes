#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"
#include "datagen.h"

/* ==== predict.cpp ====
 * this is the main module of a small C++ program written to implement
 * MCMC-based heirarchical modeling and get a better handle on the 
 * problem before attempting to implement it in the MongoDB kernel
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 6.9.2014
 */

// histogram to be stored as globals for exploration


double _alpha = 0.45;               // universal damping term

Histogram hist;

// allocates memory for and initializes global histogram
void initHistogram() 
{
    

}

// merges buckets start and end (asserting they are adjacent)
int merge (int start, int end) 
{
    assert(abs(start - end) == 1 && "invalid merge bounds");
    return 1; 
}

// splits a bucket if one fits the threshold criteria
int split ()
{
    return 1;
}

// splits and merges histograms
void restructure ()
{
    // checks threshold conditions and does stuff.

}


// recalibrates histogram based on batch input
void update (RangeSummary data)
{
    // estimate the result size of the selection using current data
    double est = 0;
    double upFracs [hist.nBuckets];

    for (int i = 0; i < hist.nBuckets; i++) {
        double minIntersect = std::max(data.low, hist.bounds[i].low);
        double maxIntersect = std::min(data.high, hist.bounds[i].high);
        
        double frac = std::max(0.0, (maxIntersect - minIntersect) / 
                               (hist.bounds[i].high - hist.bounds[i].low));
        
        upFracs[i] = (maxIntersect - minIntersect + 1) / 
                            (hist.bounds[i].high - hist.bounds[i].low + 1);
                            
        est += frac * hist.values[i];
    }
    // compute absolute estimation error
    double esterr = data.nReturned - est;              // error term

    // distribute error amongst buckets in proportion to frequency
    for (int i = 0; i < hist.nBuckets; i++) {       
        hist.values[i] = std::max((hist.values[i] + _alpha * esterr *
                                    upFracs[i] * hist.values[i] / est),
                             0.0);
    }

}

// runs the predefined test script
int main(int argc, char ** argv) 
{
    Histogram ts (4); 
    for(int i = 0; i < 4; i++) {
        ts.values[i] = i;
        ts.bounds[i].low = i;
        ts.bounds[i].high = i + 1;
    }
    std::cout << "helloworld" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << ts << std::endl;

    DataStream stream (std::string("../data/ts.in"));
    RangeSummary sum ;

    if(stream.hasNextBlock()) {
        stream.getNextBlock(&sum);
    }
    std::cout << sum << std::endl;

    std::cout << "================================" << std::endl;
    std::cout << ts << std::endl;

    return 0;
}













