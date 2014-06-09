#ifndef DATAGEN_H               // HEADER GUARD
#define DATAGEN_H 1

#include <iostream>
#include "histogram.h"

using namespace std;

/* ==== datagen.h ====
 * simulator for input of selection data from running query engine.
 * will return batch data about query execution (selectivity and cost
 * of operators) while the query engine is running.
 *
 */

class DataStream 
{
    FILE* channel;
    int batchSize;

    public:
        int openStream(char* filename);
        bool hasNextBlock();
        Histogram* getNextBlock();

}









/* Opens a channel for batch data read.
 *
 *
 *
 */




#endif
