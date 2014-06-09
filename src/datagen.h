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
        DataStream(char* filename, int btSize=10);
        ~DataStream();
        bool hasNextBlock();
        Histogram* getNextBlock();

}


#endif
