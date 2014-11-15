#!/usr/bin/env python3
import sys
from collections import defaultdict

from util import disect_packet

err_reset = "[Error] Connection reset: <{}, {}> @{}s {}:{}"
err_noext = "[Error] Connection noexist: <{}, {}> @{}s {}:{}"

def aggregate(fn):
    conns = {}
    st = []
    for line in open(fn):
        if line.startswith('[PKT] <10'):
            src, dst, t, i, s, q = disect_packet(line)
            if q == 0:
                if (src,i,s) not in conns:
                    conns[(src,i,s)] = t
                else:
                    s = err_reset.format(src, dst, t, i, s)
                    print(s, file=sys.stderr)
            elif q == 339:
                if (src,i,s) in conns:
                    dt = t - conns[(src,i,s)]
                    st.append(dt)
                    del conns[(src,i,s)]
                else:
                    s = err_noext.format(src, dst, t, i, s)
                    print(s, file=sys.stderr)
    return st


def dump(st, fn):
    out = open(fn, 'wb')
    for e in st:
        s = '{:.2f}\n'.format(e)
        out.write(bytes(s, 'UTF-8'))


def main():
    infn = sys.argv[1]
    oufn = sys.argv[2]
    st = aggregate(infn)
    dump(st, oufn)


if __name__ == '__main__':
    main()
