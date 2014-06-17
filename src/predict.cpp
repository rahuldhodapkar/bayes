#include <iostream>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include "histogram.h"
#include "datagen.h"
#include "run.h"

/* ==== predict.cpp ====
 * this is the main module of a small C++ program written to implement
 * MCMC-based heirarchical modeling and get a better handle on the 
 * problem before attempting to implement it in the MongoDB kernel
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 6.9.2014
 */

double _nSamples = 2000;            // number of samples to parse in each workload

// allocates memory for and initializes global histogram
double predict(std::string inFile, Histogram& hist)  
{
    RangeSummary sum ;
    DataStream stream (inFile);

    double totErr = 0;
    int nUsed = 0;
    for (int i = 0; i < _nSamples; i++) {  
        if(stream.hasNextBlock()) {
            stream.getNextBlock(sum);
        }       
        Bounds queryBounds (sum.low, sum.high);
        double temp = hist.getFreqOnRange(queryBounds);
        if (sum.nReturned > 0) {
            double error = std::abs(temp - sum.nReturned) / sum.nReturned;
            nUsed++;
            totErr += error;
        }
    }
    double avgErr = totErr / nUsed;
    return avgErr;
}

// runs the predefined test script
int main(int argc, char** argv) 
{
    time_t starttime, endtime;
    time(&starttime);
    // define magic numbers
    int nSamples = _nSamples;
    int nBins = 20;
    double lowVal = 0;
    double highVal = 100;
    int initVal = 10;           // initialization value
    int restructureInterval = 200;
    
    std::ofstream out;
    out.open ("../out/hist.out");

    std::ofstream timeseries;
    timeseries.open ("../out/timeseries.out");

    // build histogram
    Histogram ts (nBins, initVal, lowVal, highVal); 

    // run tuning
    RangeSummary sum ;
    DataStream stream (std::string("../data/gen.in"));
   
    // main update/restructuring code
    for (int i = 0; i < nSamples; i++) {  
        if(stream.hasNextBlock()) {
            stream.getNextBlock(sum);
        }       
        ts.update(sum);
        if (i % restructureInterval == restructureInterval - 1) {
            ts.restructure();
        }
         
        // timeseries << i <<","<<predict(std::string("../data/test.in"), ts) << std::endl;
    }
    out << ts;

    time(&endtime);
    printf("%ld millis elapsed", endtime - starttime);
    return 0;
}


