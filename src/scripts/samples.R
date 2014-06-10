# ==== samples.R ====
# This is an R script designed to emulate the actual distribution 
# of data in a database on some arbitrary key.  This data will be used
# to generate random range queries, which are then passed to a 
# self-tuning histogram as described in (Aboulnaga and Chaudhuri, 1999)
#
# @author Rahul Dhodapkar (krishnakid)
# @version 6.10.2014


truth <- c(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      # define truth
nSamples <- 50;                                 # number of samples

lbnd <- sample(1:10, nSamples, replace=TRUE);   # can be extended to runif
rbnd <- sample(1:10, nSamples, replace=TRUE);

ranges <- matrix(, nSamples, 3); 

for (i in 1:nSamples) {
    ranges[i,1] <- min(lbnd[i], rbnd[i]);
    ranges[i,2] <- max(lbnd[i], rbnd[i]);
    ranges[i,3] <- sum(truth[ranges[i,1]:ranges[i,2]]);
}
# ranges now complete, print to file.

write.table(ranges, "../../data/gen.in", sep=",", 
            row.names=FALSE, col.names=FALSE);


