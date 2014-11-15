#!/usr/bin/env python3
import sys
from collections import defaultdict

from util import disect_packet, get_src

def aggregate(fn):
    capacity = defaultdict(int)
    evict = 0
    for line in open(fn):
        if line.startswith('[PKT] <0'):
            src, dst, t, i, s, q = disect_packet(line)
            if q == 0:
                if capacity[src] < 1000:
                    capacity[src] += 1
                else:
                    evict += 1
        elif line.startswith('[CLN]'):
            src = get_src(line)
            capacity[src] -= 1
            evict += 1
    return evict


def main():
    infn = sys.argv[1]
    c = aggregate(infn)
    print('Number of cache evictions: {}'.format(c))


if __name__ == '__main__':
    main()
