#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Simple benchmark measuring randomness of three permutation functions.
random.shuffle vs numpy.random.shuffle vs numpy.random.permutation
"""
import sys
import random
from collections import defaultdict
import math

import numpy as np


def benchmark(r):
    rsh = defaultdict(int)
    nsh = defaultdict(int)

    r1 = range(r)
    r2 = range(r)
    limit = math.factorial(r-1) / 2
    for i in xrange(limit):
        random.shuffle(r1)
        np.random.shuffle(r2)

        rsh[tuple(r1)] += 1
        nsh[tuple(r2)] += 1
    return ((len(rsh), max(rsh.itervalues()), min(rsh.itervalues())),
            (len(nsh), max(nsh.itervalues()), min(nsh.itervalues())))


def plot(r, iterable):
    s = ['rshufle', 'nshufle', 'permute']
    limit = math.factorial(r-1) / 2
    fac = limit * 2 * r
    for i,it in enumerate(iterable):
        print "%s: %d/%d/%d, %d - %d = %d" % (s[i], it[0], limit, fac, it[1],
                                           it[2], it[1]-it[2])


def main():
    N = 10**4
    r = int(sys.argv[1])
    limit = math.factorial(r-1) / 2
    print 'range [%d]:' % r,
    sum = np.array([0] * 2)
    deviation = np.array([0] * 2)
    for i in xrange(N):
        iterable = benchmark(r)
        for j, it in enumerate(iterable):
            sum[j] += it[0]
            deviation[j] += it[1] - it[2]
    print 'random.shuffle: [mean] %.3f/%d, [deviation] %.3f' % (
        sum[0]*1.0/N,
        limit,
        deviation[0]*1.0/N
    )
    print '\t   numpy.shuffle : [mean] %.3f/%d, [deviation] %.3f' % (
        sum[1]*1.0/N,
        limit,
        deviation[1]*1.0/N
    )


if __name__ == '__main__':
    main()
