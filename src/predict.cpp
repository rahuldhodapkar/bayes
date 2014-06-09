#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"


/* ==== predict.cpp ====
 * this is the main module of a small C++ program written to implement
 * MCMC-based heirarchical modeling and get a better handle on the 
 * problem before attempting to implement it in the MongoDB kernel
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 6.9.2014
 */

using namespace std;

// histogram to be stored as globals for exploration

Histogram colData;



// allocates memory for and initializes global histogram
void buildHistogram() 
{


}

// frees memory for global histogram
void destroyHistogram()
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
void update (Histogram data)
{
    // incomplete function
}

// runs the predefined test script
int main(int argc, char ** argv) 
{
    cout << "helloworld" << endl;
    return 0;
}


