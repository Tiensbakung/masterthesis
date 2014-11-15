#!/usr/bin/R --slave
pdf("/tmp/lyzer/france-rate.pdf")
foo <- function(fn) {
    data = read.table(fn, header=T)
    hit = cumsum(data$hit)
    miss = cumsum(data$miss)
    rep = cumsum(data$rep)
    data$rate = (1+hit) / (hit + miss + rep + 1)
    l = strsplit(fn, "data-", fixed = T)
    data$type = l[[1]][2]
    return(data)
}

df1 = foo("france/data-proxy-1000")
df2 = foo("france/data-block-1000")
df3 = foo("france/data-cluster-1000")
## df1 = foo("atlanta/data-5-90-1000")
## df2 = foo("atlanta/data-5-60-1000")
## df2 = foo("atlanta/data-5-30-1000")
## df3 = foo("atlanta/data-10-50-1000")
## df5 = foo("atlanta/data-10-80-1000")
df4 = foo("france/data-10-80-1000")

cols=c("black", "green", "red", "blue", "purple", "yellow")
plot(NA, xlim = c(0,3600), ylim = c(0.1,0.65), xlab="Time (in seconds)", ylab="cache hit rate", main="Cumulative cache hit rate")
## selector = df1$time>5000 & df1$time < 5002
lines(df1$time, df1$rate, type="l", lty=1, col=cols[1])
lines(df2$time, df2$rate, type="l", lty=2, col=cols[2])
lines(df3$time, df3$rate, type="l", lty=3, col=cols[3])
lines(df4$time, df4$rate, type="l", lty=4, col=cols[4])
## lines(df5$time, df5$rate, type="l", lty=5, col=cols[5])
## lines(df6$time, df6$rate, type="l", lty=6, col=cols[6])
legend("bottomright", c("Proxy", "Segment Proxy", "Cluster", "Stats-10-80"), lty=c(1,2,3,4), col=cols[c(1,2,3,4)])
dev.off()
## data = read.table("/tmp/lyzer/lens-proxy")
## selector <- data$V1==0
## plot(log(data$V2), log(data$V3), type="p")
