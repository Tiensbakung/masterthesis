#!/usr/bin/env python
# -*- coding: utf-8 -*-
''' Python script for concatenating video id and duration files line
by line obtained from extracting sample data from youtube.
'''


def concat(filename):
    '''Video ids stored in tmp file.
    Durations stored in durations file. Duration  is in [mm:]ss,
    and will to be converted to seconds.
    '''
    f = open('tmp', 'rb')
    wf = open(filename, 'wb')
    for line in open('durations', 'rb'):
        i = f.readline().rstrip()
        t = line.rstrip().split(':')
        if len(t) == 1:
            d = int(t[0])
        elif len(t) == 2:
            d = int(t[0]) * 60 + int(t[1])
        elif len(t) == 3:
            d = int(t[0]) * 3600 + int(t[1]) * 60 + int(t[2])
        else:
            print "Error: two many values in", t
        wf.write(i + ' ' + str(d) + '\n')
        f.close()
        wf.close()


if __name__ == '__main__':
    concat('videodata')
