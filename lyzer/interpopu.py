#!/usr/bin/env python3
import sys
from collections import defaultdict
import re


def extract(fn):
    lens = defaultdict(lambda: defaultdict(int))
    for line in open(fn):
        x = re.match(r'10\.(\d+)', line[7:])
        if x:
            gate = x.group(1)
            y = re.search(r'(\d+):(\d+)', line[40:])
            lens[gate][y.group(2)] += 1
    return lens


def dump(lens, fn):
    oufn = open(fn, 'wb')
    for g,S in lens.items():
        for l,freq in S.items():
            s = '{} {} {}\n'.format(g, l ,freq)
            oufn.write(bytes(s, 'UTF-8'))


def main():
    infn = sys.argv[1]
    oufn = sys.argv[2]
    lens = extract(infn)
    dump(lens, oufn)


if __name__ == '__main__':
    main()
