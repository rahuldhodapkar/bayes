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

Histogram colData;



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
void update (Histogram data)
{
    // incomplete function
}

// runs the predefined test script
int main(int argc, char ** argv) 
{
    Histogram ts (4); 
    for(int i = 0; i < 4; i++) {
        ts.values[i] = i;
        ts.bounds[i].start = i;
        ts.bounds[i].end = i + 1;
    }
    std::cout << "helloworld" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << ts << std::endl;

    DataStream stream (std::string("../data/ts.in"));
    if(stream.hasNextBlock()) {
        stream.getNextBlock(&ts);
    }

    std::cout << "================================" << std::endl;
    std::cout << ts << std::endl;

    return 0;
}


