# ==== visualize.R ====
# visualization package for self-tuning histogram built in the 
# rest of this module.  Reads in information from CSV data dump 
# formatted as:
#   <START_BIN>, <STOP_BIN>, <BIN_FREQ>
#
# @author Rahul Dhodapkar (krishnakid)
# @version 6.12.14

# we will be drawing the histogram as a line plot for maximum effect.

x <- numeric();
y <- numeric();

xt <- numeric();
yt <- numeric();

raw = read.csv("../out/hist.out", header=FALSE);
truth = read.csv("../data/gen.truth", header=FALSE);

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

    x = c(x, stps[i]);
    y = c(y, vals[i]);
}

truthRes = 1;

tvals <- numeric();
for (i in 1:(100/truthRes))
{
    tvals = c(tvals, length(truth[truth >= (i-1)*truthRes &
                                  truth < i*truthRes]));
}

tvals = tvals/truthRes;

for (i in 1:length(tvals))
{
    xt = c(xt, truthRes*(i-1));
    yt = c(yt, tvals[i]);

    xt = c(xt, truthRes*i);
    yt = c(yt, tvals[i]);
}

png("../figures/histfit.png");
plot(xt,yt,type="l", xlab = "value", ylab = "# in range");
lines(x,y,col="blue");
dev.off();

