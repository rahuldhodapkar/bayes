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
low <- 0;
high <- 100;

# vals = rnorm(nSamples, mean=50, sd=20);        # normal
# vals = rcauchy(nSamples, location = 50, scale=9);     # cauchy

# vals = rbinom(nSamples, size=25, prob=0.2);    # binomial

# vals = sample(c(40, 40, 40), nSamples,
#                         replace=TRUE);

vals = sample(c(40, 40, 40, 24, 24, 24, 66.3, 66.3), nSamples,
                         replace=TRUE);

#vals = sample(c(40, 40, 40, 24, 24, 24, 66.3, 66.3, 90, 15), nSamples,
#                        replace=TRUE);


lbnd <- runif(nSamples, min=low, max=high);
rbnd <- runif(nSamples, min=low, max=high);

ranges <- matrix(, nSamples, 3);

for (i in 1:nSamples) {
    ranges[i,1] <- min(lbnd[i], rbnd[i]);    
    ranges[i,2] <- max(lbnd[i], rbnd[i]);    

    ranges[i,3] <- length(vals[vals > ranges[i,1] & vals < ranges[i,2]]);
}

write.table(ranges, "../data/gen.in", sep=",", 
            row.names=FALSE, col.names=FALSE);

write.table(vals, "../data/gen.truth", sep=",",
            row.names=FALSE, col.names=FALSE);

