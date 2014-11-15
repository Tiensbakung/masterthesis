#!/bin/sh
echo "constructing URL list..."
sed "s/^/www.youtube.com\//g" videos > video-list
echo "start batch fetching video durations..."
youtube-dl -f 5/133/242 --get-duration -i --default-search auto -a video-list > durations
