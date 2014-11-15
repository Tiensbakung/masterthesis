import re

def get_time(line):
    b = line.index('@')
    e = line.index('.', b) + 3
    return line[b+1:e]


def get_video(line):
    x = re.search('(\d+):(\d+)', line)
    return int(x.group(1)), int(x.group(2))


def disect_packet(line):
    x = re.match(r'\[PKT] <(\d+\.\d+\.\d+\.\d+), (\d+\.\d+\.\d+\.\d+)> @(\d+\.\d+)s: Receive packet (\d+):(\d+):(\d+)', line)
    return x.group(1), x.group(2), float(x.group(3)), int(x.group(4)), int(x.group(5)), int(x.group(6))


def get_src(line):
    x = re.match(r'(\d+\.\d+\.\d+\.\d+)', line[7:])
    return x.group(1)