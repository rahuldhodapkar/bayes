#ifndef RUN_H                       // header guard
#define RUN_H 1

#include <list>
#include <utility>
#include <string>

typedef std::pair<double, double> Bounds;

class Run {
    std::list<int> buckets;
    Bounds freqBounds;
    Bounds rangeBounds;
    double totalFreq;

    public:
        // data functions
        double getHiFreq ();
        double getLoFreq ();
        double getTotalFreq () const;
        Bounds getRangeBounds () const;

        void setRangeBounds(Bounds);
        void setTotalFreq(double);

        bool isMerged () const;
        void split(std::list<Run>& run);
        void merge(Run& run);
        double getMinDiff (Run& run);
        std::list<int> getBuckets ();

        // debug functions
        void printBuckets();

        // constructor/destructor
        Run(int bucket, double freq, Bounds bounds);
        Run();
        ~Run();
};

typedef std::list<Run>::iterator RunIter;

#endif
