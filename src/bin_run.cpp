#include "bin_run.h"
#include <list>
#include <utility>
#include <string>
#include <iostream>

/* ==== run.cpp ====
 * Implementation for the Run class defined in run.h.  Handles the
 * data structures used during histogram restructuring
 *
 * @author Rahul Dhodapkar
 * @version 6.12.14
 */


Run::Run(int bucket, double freq, std::pair<double, double> bounds) {
    freqBounds.first = freqBounds.second = totalFreq = freq;
    rangeBounds.first = bounds.first;
    rangeBounds.second = bounds.second;
    buckets.push_back(bucket);
}

Run::Run() {

}

Run::~Run() { 
}

double Run::getHiFreq() {
    return freqBounds.second;
}

double Run::getLoFreq() { 
    return freqBounds.first;
}

double Run::getTotalFreq() const { 
    return totalFreq;
}

bool Run::isMerged() const { 
    return buckets.size() > 1;
}

std::list<int> Run::getBuckets() { 
    return buckets;
}

// return minimum of maximum differences.
double Run::getMaxDiff (Run& run) {
    return std::max(run.getHiFreq() - getLoFreq(),
                    getHiFreq() - run.getLoFreq());
}

Bounds Run::getRangeBounds () const { 
    return rangeBounds;
}

void Run::setRangeBounds(Bounds nwBnds) {
    rangeBounds.first = nwBnds.first;
    rangeBounds.second = nwBnds.second;
}

void Run::setTotalFreq(double freq) { 
    totalFreq = freq;
}

// split information from local Run across external runs
void Run::split (std::list<Run>& runs) {
    int nNew = runs.size();

    double newFreq = getTotalFreq() / (nNew + 1) ;
    double rangeStep = (rangeBounds.second - rangeBounds.first) /
                                                     (nNew + 1);
    double curStart = rangeBounds.first;
    for (RunIter i = runs.begin(); i != runs.end(); i++) { 
        i->setTotalFreq(newFreq);
        i->setRangeBounds(Bounds(curStart, curStart+rangeStep));
        curStart += rangeStep;
    }

    // update main object and deal with rounding errors
    setTotalFreq(newFreq);
    setRangeBounds(Bounds(curStart, rangeBounds.second));
}

// merge two runs and store info in first run.
void Run::merge (Run& run) { 
    // merge bucket ranges
    std::list<int> exBuckets = run.getBuckets();
    buckets.splice(buckets.end(), exBuckets);    
    totalFreq += run.getTotalFreq();
    freqBounds.first = std::min(getLoFreq(), run.getLoFreq());
    freqBounds.second = std::max(getHiFreq(), run.getHiFreq());
    
    Bounds extBounds = run.getRangeBounds();
    rangeBounds.first = std::min(extBounds.first, rangeBounds.first);
    rangeBounds.second = std::max(extBounds.second, rangeBounds.second);
}


void Run::printBuckets() {
    typedef std::list<int>::iterator Iter;
    std::cout << "[" ;
    for(Iter i = buckets.begin(); i != buckets.end(); i++) {
       std::cout << *i << ", " ;
    }
    std::cout << std::string("]");
}

