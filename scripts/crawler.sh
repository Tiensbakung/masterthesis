#!/bin/sh
wget -l 5 --no-parent --accept-regex="/watch\?v=\w{11}" -rq "www.youtube.com"
