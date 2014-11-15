#!/usr/bin/R --slave
pdf("/tmp/lyzer/france-video.pdf")
video <- function(fn) {
    data = read.table(fn, header=T)
    l = strsplit(fn, "video-", fixed = T)
    data$type = l[[1]][2]
    return(data)
}

segment <- function(fn) {
    data = read.table(fn, header=T)
    l = strsplit(fn, "segment-", fixed = T)
    data$type = l[[1]][2]
    return(data)
}

videoProxy = video("france/video-proxy")
videoBlock = video("france/video-block")
videoCluster = video("france/video-cluster")
## videoProxy = video("atlanta/video-5-30-1000")
## videoBlock = video("atlanta/video-5-90-1000")
## videoCluster = video("atlanta/video-10-50-1000")
video1070 = video("france/video-10-80")


## segmentProxy = segment("france/segment-proxy")
## segmentBlock = segment("france/segment-block")
## segmentCluster = segment("france/segment-cluster")
## segment1080 = segment("france/segment-10-80")

par(mfrow = c( 2, 2))
## plot(videoProxy$index, videoProxy$miss+videoProxy$rep+videoProxy$hit, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Proxy", lty=3)
## lines(videoProxy$index, videoProxy$miss+videoProxy$rep, type="l")
## legend("topright", c("Total", "Miss"), lty=c(3,1))

## plot(videoBlock$index, videoBlock$miss+videoBlock$rep+videoBlock$hit, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Segment Proxy", lty=3)
## lines(videoBlock$index, videoBlock$miss+videoBlock$rep, type="l")
## legend("topright", c("Total", "Miss"), lty=c(3,1))

## plot(videoCluster$index, videoCluster$miss+videoCluster$rep+videoCluster$hit, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Cluster", lty=3)
## lines(videoCluster$index, videoCluster$miss+videoCluster$rep, type="l")
## legend("topright", c("Total", "Miss"), lty=c(3,1))

## plot(video1070$index, video1070$miss+video1070$rep+video1070$hit, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Stats-10-80", lty=3)
## lines(video1070$index, video1070$miss+video1070$rep, type="l")
## legend("topright", c("Total", "Miss"), lty=c(3,1))



## plot(videoProxy$index, videoProxy$miss+videoProxy$rep, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Proxy")
## plot(videoBlock$index, videoBlock$miss+videoBlock$rep, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Segment Proxy")
## plot(videoCluster$index, videoCluster$miss+videoCluster$rep, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Cluster")
## plot(video1070$index, video1070$miss+video1070$rep, type="l", xlab = "Video Index", ylab = "Number of cache misses", main="Stats-10-80")





plot(segmentProxy$segment, segmentProxy$miss+segmentProxy$rep, type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Proxy")
plot(segmentBlock$segment, segmentBlock$miss+segmentBlock$rep, type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Segment Proxy")
plot(segmentCluster$segment, segmentCluster$miss+segmentCluster$rep, type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Cluster")
plot(segment1080$segment, segment1080$miss+segment1080$rep, type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Stats-10-80")
rate <- function(data) {
    s = sum(data$miss)+sum(data$rep)
    r = (data$miss+data$rep)/s
}
## plot(segmentProxy$segment, rate(segmentProxy), type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Proxy")
## plot(segmentBlock$segment, rate(segmentBlock), type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Segment Proxy")
## plot(segmentCluster$segment, rate(segmentCluster), type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Cluster")
## plot(segment1080$segment, rate(segment1080), type="l", xlab = "Segment Index", ylab = "Number of cache misses", main="Stats-10-80")

cols=c("black", "green", "red", "blue", "purple", "yellow")
dev.off()
