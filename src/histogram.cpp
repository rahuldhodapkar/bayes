#include <stdlib.h>
#include <stdio.h>
#include <utility>

#include "histogram.h"
#include "run.h"

/* ==== histogram.cpp ====
 * implementation for the histogram class datatype laid out in histogram.h
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 6.12.14
 */

double _alpha = 0.5;                // universal damping term

double _mergeThreshold = 0.00025;      // merge threshold parameter

double _splitThreshold = 0.1;       // split threshold parameter

// Histogram constructor
Histogram::Histogram(int size, double binInit, double lowBound, double highBound)
{
    nBuckets = size;
    values = new double[nBuckets];    
    bounds = new Bounds[nBuckets];
    nObs = 0;
    
    // initialize
    double curStart = lowBound;
    double stepSize = (highBound - lowBound) / nBuckets; 

    for (int i = 0; i < nBuckets - 1; i++) {
        values[i] = binInit;
        bounds[i].first = curStart;
        bounds[i].second = curStart + stepSize;
        curStart += stepSize;
    }
    values[nBuckets - 1] = binInit;
    bounds[nBuckets - 1].first = curStart;
    bounds[nBuckets - 1].second = highBound;
}

// Histogram destructor
Histogram::~Histogram()
{
    delete[] values;
    delete[] bounds;
}

bool Histogram::rangeBoundOrderingFunction  (const Run& run1, const Run& run2) {
    return run1.getRangeBounds().first < run2.getRangeBounds().first;
}

bool Histogram::splitOrderingFunction (const Run& run1, const Run& run2) {
    if (run1.isMerged() && run2.isMerged()) {
        return run1.getTotalFreq() > run2.getTotalFreq();
    } else if (run1.isMerged()) {
        return false;
    } else if (run2.isMerged()) {
        return true;
    } else {
        return run1.getTotalFreq() > run2.getTotalFreq();
    }
}

// recalibrates histogram based on batch input
void Histogram::update(RangeSummary& data)
{
    nObs++;

    // estimate the result size of the selection using current data
    double est = 0;
    bool doesIntersect [nBuckets];
    double dIntersect = 0;
    double minIntersect, maxIntersect;

    for (int i = 0; i < nBuckets; i++) {
        minIntersect = std::max(data.low, bounds[i].first);
        maxIntersect = std::min(data.high, bounds[i].second);
       
        double intersectFrac = std::max((maxIntersect - minIntersect) /
                     (bounds[i].second - bounds[i].first), 0.0);
    
        doesIntersect[i] = intersectFrac > 0;

        est += values[i] * intersectFrac;
    }

    // compute absolute estimation error
    double esterr = data.nReturned - est;              // error term
    
    // distribute error amongst buckets in proportion to frequency
    for (int i = 0; i < nBuckets; i++) {     
        minIntersect = std::max(data.low, bounds[i].first);
        maxIntersect = std::min(data.high, bounds[i].second);
        double frac = (maxIntersect - minIntersect + 1) /
                      (bounds[i].second - bounds[i].first + 1);
        
        if (doesIntersect[i]) {
            values[i] = std::max(0.0, values[i] + 
                                        (frac * _alpha * esterr *
                                        values[i] /(est)));
        }
    }

}

void Histogram::restructure () {
    typedef std::pair<double, double> Bounds;
    typedef std::list<Run>::iterator RunIter;
    typedef std::pair<RunIter, RunIter> MergePair;

    // initialize B runs of buckets
    int nBins = nBuckets;
    std::list<Run> runs;
    double totalFreq = 0;

    for(int i = 0; i < nBins; i++) {
        totalFreq += values[i];
        Run nr (i, values[i], 
                 Bounds(bounds[i].first, bounds[i].second));
        runs.push_back(nr); 
    }
    
    // for every two consercuive runs of buckets, find the maximum
    // difference in frequency between a bucket in the first run and
    // a bucket in the second run
    std::list<Run> reclaimed;
    bool mergeComplete = false;

    while (!mergeComplete) { //  && reclaimed.size() + 1 < runs.size() 
        mergeComplete = true;
        MergePair best;
        double minDiff = std::numeric_limits<double>::infinity();
        RunIter i = runs.begin();
        RunIter curRun = i;
        
        int nUnmerged = 0;

        for (i++; i != runs.end(); i++) {
            double cDiff = curRun->getMaxDiff(*i); 
           
            if (cDiff < minDiff) { 
                best.first = curRun;
                best.second = i;
                minDiff = cDiff;
            }

            if (!(curRun->isMerged())) {
                nUnmerged++;
            }

            curRun = i;
        }

        if (nUnmerged <= reclaimed.size()) {
            break;                                   // otherwise merged buckets will be split
        }

        if (minDiff < (_mergeThreshold * totalFreq)) {
            best.first->merge(*(best.second));          // merge
            reclaimed.push_back(*(best.second));       // reclaim Run
            runs.erase(best.second);
            mergeComplete = false;
        }
        // and repeat.
    }
    // split until appropriate
    //
    // if a < b then a will come before b in the listing
    //
    runs.sort(splitOrderingFunction);
    
    int nToSplit = nBins * _splitThreshold;

    std::list<Run> candidates;
    for (RunIter i = runs.begin(); 
        candidates.size() <= nToSplit && i != runs.end(); i++) {
        candidates.push_front(*i);          // for reverse access later

        i = runs.erase(i);
        i--;
    }

    double fullFreq = 0;

    for(RunIter i = candidates.begin(); i != candidates.end(); i++) {
        fullFreq += i->getTotalFreq();
    }

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

    runs.sort(rangeBoundOrderingFunction);

    // mappy map map
    int counter = 0;
    for (RunIter i = runs.begin(); i != runs.end(); i++) { 
        values[counter] = i->getTotalFreq();
        bounds[counter].first = i->getRangeBounds().first;
        bounds[counter].second = i->getRangeBounds().second;
        counter++;
    }
}

double Histogram::getFreqOnRange(Bounds& range)
{
    double freq = 0;
    for (int i = 0; i < nBuckets; i++) {
        double overlap = std::min(range.second, bounds[i].second) -
                         std::max(range.first, bounds[i].first);
        overlap = std::max(overlap / (bounds[i].second-bounds[i].first), 0.0);
        freq += overlap * values[i];
    }
    return freq;
}

std::ostream& operator<<(std::ostream &strm, const Histogram &hist) {
    std::ostream& retStrm = strm;
    for(int i = 0; i < hist.nBuckets; i++) {
        retStrm << hist.bounds[i].first << "," 
                << hist.bounds[i].second << ","
                << hist.values[i] << std::endl;
    }
    return retStrm;
}
