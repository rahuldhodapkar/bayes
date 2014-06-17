# ==== samples.R ====
# This is an R script designed to emulate the actual distribution 
# of data in a database on some arbitrary key.  This data will be used
# to generate random range queries, which are then passed to a 
# self-tuning histogram as described in (Aboulnaga and Chaudhuri, 1999)
#
# @author Rahul Dhodapkar (krishnakid)
# @version 6.10.2014

# build a ground truth histogram

nSamples <- 10000;
nQueries <- 2000;
low <- 0;
high <- 100;

genQueries <- function(vals) {
    lbnd <- runif(nQueries, min=low, max=high);
    rbnd <- runif(nQueries, min=low, max=high);

    dir <- sample(c(TRUE, FALSE), nQueries, replace=TRUE);

    ranges <- matrix(, nQueries, 3);

    for (i in 1:nQueries) {
        if (TRUE) { 
            ranges[i,1] = min(lbnd[i], rbnd[i]);
            ranges[i,2] = max(lbnd[i], rbnd[i]);
        } else if (dir[i]) {               # forward direction
            if(rbnd[i] < lbnd[i]) {
                ranges[i,1] = low;
                ranges[i,2] = lbnd[i];
            } else {
                ranges[i,1] = lbnd[i];
                ranges[i,2] = rbnd[i];
            }
        } else { 
            if(lbnd[i] > rbnd[i]) {
                ranges[i,1] = rbnd[i];
                ranges[i,2] = high;
            } else {
                ranges[i,1] = lbnd[i];
                ranges[i,2] = rbnd[i];
            }
        }

        ranges[i,3] <- length(vals[vals > ranges[i,1] & vals < ranges[i,2]]);
    }
    return(ranges);
}


# trainVals = rnorm(nSamples, mean=50, sd=20);        # normal
# testVals = rnorm(nSamples, mean=50, sd=20);        # normal

trainVals = read.csv("../data/zipf1.dist", header=FALSE);
testVals = read.csv("../data/zipf2.dist", header=FALSE);

trainVals = trainVals[,1];
testVals = testVals[,1];

warnings();

# vals = rcauchy(nSamples, location = 50, scale=9);     # cauchy

# trainVals = rbinom(nSamples, size=65, prob=0.5);    # binomial
# testVals = rbinom(nSamples, size=65, prob=0.5);    # binomial

# vals = sample(c(40, 40, 40), nSamples,
#                         replace=TRUE);

# vals = sample(c(40, 40, 40, 24, 24, 24, 66.3, 66.3), nSamples,
#                         replace=TRUE);

#vals = sample(c(40, 40, 40, 24, 24, 24, 66.3, 66.3, 90, 15), nSamples,
#                        replace=TRUE);
trainRanges = genQueries(trainVals);
testRanges = genQueries(testVals);

write.table(trainRanges, "../data/gen.in", sep=",", 
            row.names=FALSE, col.names=FALSE);

write.table(trainVals, "../data/gen.truth", sep=",",
            row.names=FALSE, col.names=FALSE);

write.table(testRanges, "../data/test.in", sep=",",
            row.names=FALSE, col.names=FALSE);

write.table(testRanges, "../data/test.truth", sep=",",
            row.names=FALSE, col.names=FALSE);

