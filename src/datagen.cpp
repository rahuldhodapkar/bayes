#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

#include "histogram.h"
#include "datagen.h"

/* ==== datagen.cpp ====
 * implementation of the dummer input selection module defined in 
 * datagen.h for the self-tuning histogram proof of concept project.
 */

// constructor opens file as input channel
DataStream::DataStream(std::string filename, int btSize)
{
    batchSize = btSize;
    channel.open(filename);
}

// destructor closes file
DataStream::~DataStream()
{
    channel.close();
}

// "true" if file is not at end-of-file
bool DataStream::hasNextBlock()
{
    return (channel.is_open());
}

// loads the next block of statistical data into
// the histogram pointer specified by "out"
void DataStream::getNextBlock(Histogram* out)
{
    int nBuckets = out->nBuckets;
    // ugly, but will be replaced when hooked into query execution
    for(int i = 0; i < nBuckets && hasNextBlock(); i++) {
        std::string sSt, sEnd, sVal;
        std::getline(channel, sSt, ',');
        std::getline(channel, sEnd, ',');
        std::getline(channel, sVal);

        out->bounds[i].start = std::stod(sSt);
        out->bounds[i].end = std::stoi(sEnd);
        out->values[i] = std::stoi(sVal);
    } 
}


