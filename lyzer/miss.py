#!/usr/bin/env python3
import sys
from collections import defaultdict

from util import get_video

def aggregate(fn):
    miss_i = defaultdict(int)
    miss_s = defaultdict(int)
    rep_i = defaultdict(int)
    rep_s = defaultdict(int)
    hit_i = defaultdict(int)
    hit_s = defaultdict(int)
    for line in open(fn):
        if line.startswith('[RHC]'):
            i, s = get_video(line)
            hit_i[i] += 1
            hit_s[s] += 1
        elif line.startswith('[RNH]'):
            i, s = get_video(line)
            miss_i[i] += 1
            miss_s[s] += 1
        elif line.startswith('[RCM]'):
            i, s = get_video(line)
            rep_i[i] += 1
            rep_s[s] += 1
    return hit_i, miss_i, rep_i, hit_s, miss_s, rep_s


def dump(hit_i, miss_i, rep_i, hit_s, miss_s, rep_s, fn_i, fn_s):
    out_i = open(fn_i, 'wb')
    out_i.write(bytes('index hit miss rep\n', 'UTF-8'))
    for k in sorted(set(hit_i).union(set(miss_i).union(set(rep_i)))):
        s = '{} {} {} {}\n'.format(k, hit_i[k], miss_i[k], rep_i[k])
        out_i.write(bytes(s, 'UTF-8'))
    out_i.close()
    out_s = open(fn_s, 'wb')
    out_s.write(bytes('segment hit miss rep\n', 'UTF-8'))
    for k in sorted(set(hit_s).union(set(miss_s).union(set(rep_s)))):
        s = '{} {} {} {}\n'.format(k, hit_s[k], miss_s[k], rep_s[k])
        out_s.write(bytes(s, 'UTF-8'))


def main():
    infn = sys.argv[1]
    out_i = sys.argv[2]
    out_s = sys.argv[3]
    hit_i, miss_i, rep_i, hit_s, miss_s, rep_s = aggregate(infn)
    dump(hit_i, miss_i, rep_i, hit_s, miss_s, rep_s, out_i, out_s)


if __name__ == '__main__':
    main()
