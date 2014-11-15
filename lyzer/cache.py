#!/usr/bin/env python3
import sys
from collections import defaultdict

from util import get_time


def aggregate(fn):
    hit = defaultdict(int)
    miss = defaultdict(int)
    rep = defaultdict(int)
    for line in open(fn):
        if line.startswith('[RHC]'):
            time = get_time(line)
            hit[time] +=1
        elif line.startswith('[RNH]'):
            time = get_time(line)
            miss[time] += 1
        elif line.startswith('[RCM]'):
            time = get_time(line)
            rep[time] += 1
    return hit, miss, rep


def dump(hit, miss, rep, fn):
    out = open(fn, 'wb')
    out.write(bytes('time hit miss rep\n', 'UTF-8'))
    for k in sorted(set(hit).union(set(miss).union(rep)), key=lambda x: float(x)):
        s = '{} {} {} {}\n'.format(k, hit[k],miss[k], rep[k])
        out.write(bytes(s, 'UTF-8'))


def main():
    infn = sys.argv[1]
    oufn = sys.argv[2]
    hit, miss,rep = aggregate(infn)
    dump(hit, miss, rep, oufn)


if __name__ == '__main__':
    main()
