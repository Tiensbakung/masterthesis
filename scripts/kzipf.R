#!/usr/bin/Rscript --slave
library(zipfR)
args <- commandArgs(TRUE)
set.seed(as.integer(args[1]))
ZM = lnre("zm", alpha=9/10, B=0.01)
zmsample = rlnre(ZM, n=as.integer(args[2]))
cat(as.integer(as.character(zmsample)))
