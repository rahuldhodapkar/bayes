#ifndef RUN_H                       // header guard
#define RUN_H 1

#include <list>
#include <utility>
#include <string>

/* ==== run.h ====
 * class prototype for a "run", which holds a set of contiguous bins
 * marked for merging by the self-tuning histogram, along with some
 * pertinent metadata about those bins.  The merging and splitting code
 * is also contained in the Run class. 
 *
 * @author Rahul Dhodapkar (krishnakid)
 * @version 4.12.14
 */



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
