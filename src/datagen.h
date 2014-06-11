#ifndef DATAGEN_H               // HEADER GUARD
#define DATAGEN_H 1

#include <iostream>
#include <fstream>
#include "histogram.h"

/* ==== datagen.h ====
 * simulator for input of selection data from running query engine.
 * will return batch data about query execution (selectivity and cost
 * of operators) while the query engine is running.
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 6.9.2014
 */


class RangeSummary{
    public:
        double low;
        double high;
        double nReturned;
};

class DataStream 
{
    std::ifstream channel;
    int batchSize;

    public:
        DataStream(std::string filename, int btSize=10);
        ~DataStream();
        bool hasNextBlock();
        void getNextBlock(RangeSummary* out);
};

std::ostream& operator<<(std::ostream &strm, const RangeSummary &summ);

#endif
