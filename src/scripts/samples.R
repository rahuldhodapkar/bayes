# ==== samples.R ====
# This is an R script designed to emulate the actual distribution 
# of data in a database on some arbitrary key.  This data will be used
# to generate random range queries, which are then passed to a 
# self-tuning histogram as described in (Aboulnaga and Chaudhuri, 1999)
#
# @author Rahul Dhodapkar (krishnakid)
# @version 6.10.2014


# build a ground truth histogram

nSamples <- 5000;
nBins <- 100;



vals = rnorm(nSamples, mean=50, sd=10);
# vals = rbinom(nSamples, size=40, prob=0.2);

vals = floor(vals);
vals = vals[vals > 0];
vals = vals[vals <= nBins];
truth = matrix(0, 1, nBins);

for (val in vals) {
    truth[val] <- truth[val] + 1;
}

lbnd <- sample(1:nBins, nSamples, replace=TRUE); # can be extended to runif
rbnd <- sample(1:nBins, nSamples, replace=TRUE);

ranges <- matrix(, nSamples, 3); 

for (i in 1:nSamples) {
    ranges[i,1] <- min(lbnd[i], rbnd[i]);
    ranges[i,2] <- max(lbnd[i], rbnd[i]);
    ranges[i,3] <- sum(truth[ranges[i,1]:ranges[i,2]]);
}
# ranges now complete, print to file.

write.table(ranges, "../../data/gen.in", sep=",", 
            row.names=FALSE, col.names=FALSE);

print("recall TRUTH:");
print(truth);
