# ==== fitting.R ====
# visualization package for self-tuning histogram built in the 
# rest of this module.  Reads in information from CSV data dump 
# formatted as:
#   <START_BIN>, <STOP_BIN>, <BIN_FREQ>
#
# @author Rahul Dhodapkar (krishnakid)
# @version 6.12.14

library("lsa");

# we will be drawing the histogram as a line plot for maximum effect.

x <- numeric();
y <- numeric();

timeseries = read.csv("../out/timeseries.out", header=FALSE);
raw = read.csv("../out/hist.out", header=FALSE);
truth = read.csv("../data/gen.truth", header=FALSE);

start = 1;
nBins = length(raw[,1]);

for (i in 1:1000) {
    sts = timeseries[start:(start+nBins - 1),1];
    stps = timeseries[start:(start+nBins - 1),2];
    vals = timeseries[start:(start+nBins - 1),3]/(stps - sts);

    tvals <- numeric();
    for (k in 1:length(vals))
    {
        tvals = c(tvals, length(truth[truth >= sts[k] & truth < stps[k]]));
    }
    tvals = tvals/(stps-sts);

    cosine_sim = cosine(tvals, vals);
    x <- c(x, i);
    y <- c(y, cosine_sim);

    start = start + nBins;
}

length(x) <- length(x) - 1;
length(y) <- length(y) - 1;

png("../figures/converge.png");
plot(x,y,type="l", xlab="# of iterations", ylab="cosine similarity");
dev.off();


