#include <iostream>
#include <limits>
#include <cmath>
#include <stack>
#include <list>
#include <utility>
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

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

// histogram to be stored as globals for exploration


double _alpha = 0.7;                // universal damping term

double _mergeThreshold = 0.0001;      // merge threshold parameter

double _splitThreshold = 0.1;       // split threshold parameter

// allocates memory for and initializes global histogram
void initHistogram() 
{
    

}
// recalibrates histogram based on batch input
void update (RangeSummary& data, Histogram& hist)
{
    // estimate the result size of the selection using current data
    double est = 0;
    bool doesIntersect [hist.nBuckets];
    double dIntersect = 0;

    for (int i = 0; i < hist.nBuckets; i++) {
        double minIntersect = std::max(data.low, hist.bounds[i].low);
        double maxIntersect = std::min(data.high, hist.bounds[i].high);
        
        doesIntersect[i] = maxIntersect - minIntersect > 0 ||
                             (minIntersect >= hist.bounds[i].low &&
                             maxIntersect < hist.bounds[i].high);

        // bounds intervals are [low, high).
        
        if (doesIntersect[i]) {
            est += hist.values[i];
            dIntersect += hist.bounds[i].high - hist.bounds[i].low;
        }

    }
    // compute absolute estimation error
    double esterr = data.nReturned - est;              // error term

    // distribute error amongst buckets in proportion to frequency
    for (int i = 0; i < hist.nBuckets; i++) {     
        double frac = 0;
        if(doesIntersect[i]) { 
            frac = (hist.bounds[i].high - hist.bounds[i].low) /
                                        dIntersect;
        }

        hist.values[i] = std::max(0.0, hist.values[i] + 
                                    frac * _alpha * esterr *
                                    hist.values[i] / est);
    }

}

void restructure (Histogram& hist) {
    typedef std::pair<double, double> Bounds;
    typedef std::list<Run>::iterator RunIter;
    typedef std::pair<RunIter, RunIter> MergePair;

    // initialize B runs of buckets
    int nBins = hist.nBuckets;
    std::list<Run> runs;
    double totalFreq = 0;

    for(int i = 0; i < nBins; i++) {
        totalFreq += hist.values[i];
        Run nr (i, hist.values[i], 
                 Bounds(hist.bounds[i].low, hist.bounds[i].high));
        runs.push_back(nr); 
    }

    // for every two consercuive runs of buckets, find the maximum
    // difference in frequency between a bucket in the first run and
    // a bucket in the second run
    std::list<Run> reclaimed;
    bool mergeComplete = false;

    while (!mergeComplete && reclaimed.size() + 1 < runs.size()) { 
        mergeComplete = true;
        MergePair best;
        double minDiff = std::numeric_limits<double>::infinity();
        RunIter i = runs.begin();
        RunIter curRun = i;
        for (i++; i != runs.end(); i++) {
            double cDiff = curRun->getMinDiff(*i); 
            if (cDiff < minDiff) { 
                best.first = curRun;
                best.second = i;
                minDiff = cDiff;
            }
            curRun = i;
        }

        if (minDiff < _mergeThreshold * totalFreq) {
            best.first->merge(*(best.second));          // merge
            reclaimed.push_back(*(best.second));       // reclaim Run
            runs.erase(best.second);
            mergeComplete = false;
        }
        // and repeat.
    }

    // split until appropriate
    runs.sort([] (const Run& run1, const Run& run2) {
        if (run1.isMerged() && run2.isMerged()) {
            return run1.getTotalFreq() > run2.getTotalFreq();
        } else if (run1.isMerged()) {
            return true;
        } else if (run2.isMerged()) {
            return false;
        } else {
            return run1.getTotalFreq() > run2.getTotalFreq();
        }
    });
    
    int nToSplit = nBins * _splitThreshold;

    std::list<Run> candidates;
    for (RunIter i = runs.begin(); 
         candidates.size() <= nToSplit && i != runs.end(); i++) {
        candidates.push_front(*i);          // for reverse access later
        runs.erase(i); 
    }

    double fullFreq = 0;

    std::for_each(candidates.begin(), candidates.end(), 
        [&] (const Run& val) {
            fullFreq += val.getTotalFreq();
        });

    // need to ensure that sum is full range
    int totalReclaimed = reclaimed.size();
    int candidatesProcessed = 0;

    for (RunIter i = candidates.begin(); i != candidates.end(); i++) {
        int nAlloc;
        if (candidatesProcessed < candidates.size() - 1) { 
            nAlloc = totalReclaimed * i->getTotalFreq() / fullFreq;
        } else { 
            nAlloc = reclaimed.size();  // ensure full split.
        }

        std::list<Run> updateRuns;
        for (int i = 0; i < nAlloc; i++) {
            updateRuns.push_back(*(reclaimed.begin()));
            reclaimed.erase(reclaimed.begin());
        }
        i->split(updateRuns);           // merge step

        // merge back onto candidates
        runs.push_back(*i);
        runs.splice(runs.end(), updateRuns);

        candidatesProcessed++;
    } 

    // now order by bounds and map back onto original histogram.

    runs.sort([] (const Run& run1, const Run& run2) {
        return run1.getRangeBounds().first < run2.getRangeBounds().first;
    });

    // mappy map map
    int counter = 0;
    for (RunIter i = runs.begin(); i != runs.end(); i++) { 
        hist.values[counter] = i->getTotalFreq();
        hist.bounds[counter].low = i->getRangeBounds().first;
        hist.bounds[counter].high = i->getRangeBounds().second;
        counter++;
    }
}

// runs the predefined test script
int main(int argc, char** argv) 
{
    // define magic numbers
    int nSamples = 1500;
    int nBins = 40;
    double lowVal = 0;
    double highVal = 100;
    int initVal = 10;           // initialization value
    int restructureInterval = 100;
    
    std::ofstream out;
    out.open ("../out/hist.out");

    // calculate additional magic numbers
    double stepSize = (highVal - lowVal) / nBins;

    // build histogram
    Histogram ts (nBins); 
 
    double curStart = lowVal;
    for(int i = 0; i < nBins - 1; i++) {
        ts.values[i] = initVal;
        ts.bounds[i].low = curStart;
        ts.bounds[i].high = curStart + stepSize;
        curStart += stepSize;
    }
    ts.values[nBins - 1] = initVal;    // ensure bounds fully covered.
    ts.bounds[nBins - 1].low = curStart;
    ts.bounds[nBins - 1].high = highVal;

    // run tuning
    RangeSummary sum ;
    DataStream stream (std::string("../data/gen.in"));
    
    for (int i = 0; i < nSamples; i++) {  
        if(stream.hasNextBlock()) {
            stream.getNextBlock(sum);
        }       
        update(sum, ts);
        if (i % restructureInterval == restructureInterval - 1) {
            restructure(ts);
        }
    }
    out << ts;
    return 0;
}


