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

x = timeseries[,1];
y = timeseries[,2];

png("../figures/converge.png");
plot(x,y,type="l",ylim=c(0,1), xlab="# of iterations", ylab="average relative error");
dev.off();

warnings();
