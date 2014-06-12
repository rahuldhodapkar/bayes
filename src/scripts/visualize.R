# ==== visualize.R ====
# visualization package for self-tuning histogram built in the 
# rest of this module.  Reads in information from CSV data dump 
# formatted as:
#   <START_BIN>, <STOP_BIN>, <BIN_FREQ>
#
# @author Rahul Dhodapkar (krishnakid)
# @version 6.12.14

library("ggplot2");         # load ggplot2 graphics library

# we will be drawing the histogram as a line plot for maximum effect.

x <- numeric();
y <- numeric();

xt <- numeric();
yt <- numeric();

raw = read.csv("../../out/hist.out", header=FALSE);
truth = read.csv("../../data/gen.truth", header=FALSE);

sts = raw[,1];
stps = raw[,2];
vals = raw[,3]/(raw[,2] - raw[,1]);

tvals <- numeric();
for (i in 1:length(vals))
{
    tvals = c(tvals, length(truth[truth > sts[i] & truth < stps[i]]));
}
tvals = tvals/(raw[,2] - raw[,1]);

for (i in 1:length(vals))
{
    x = c(x, sts[i]);
    y = c(y, vals[i]);

    xt = c(xt, sts[i]);
    yt = c(yt, tvals[i]);


    x = c(x, stps[i]);
    y = c(y, vals[i]);

    xt = c(xt, stps[i]);
    yt = c(yt, tvals[i]);

}

plot(xt,yt,type="l");
lines(x,y,col="blue");

