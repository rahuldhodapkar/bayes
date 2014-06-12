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

raw = read.csv("../../out/hist.out", header=FALSE);

sts = raw[,1];
stps = raw[,2];
vals = raw[,3]/(raw[,2] - raw[,1]);

for (i in 1:length(vals))
{
    x = c(x, sts[i]);
    y = c(y, vals[i]);

    x = c(x, stps[i]);
    y = c(y, vals[i]);
}

plot(x, y, type="l");
