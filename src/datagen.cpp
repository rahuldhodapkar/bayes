#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"
#include <string>


using namespace std;

/* ==== datagen.cpp ====
 * implementation of the dummer input selection module defined in 
 * datagen.h for the self-tuning histogram proof of concept project.
 */

// constructor opens file as input channel
DataStream::DataStream(char * filename, int btSize)
{
    batchSize = btSize;
    channel = fopen(filename, "r");
}

// destructor closes file
DataStream::~DataStream()
{
    fclose(channel);
}

// "true" if file is not at end-of-file
bool DataStream::hasNextBlock()
{
    return (feof(channel) == 0)
}

// loads the next block of statistical data into
// the histogram pointer specified by "out"
void DataStream::getNextBlock(Histogram* out)
{
    string line;
    getline(channel, line);
    *out()
}



