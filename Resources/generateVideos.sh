#!/bin/bash
avconv -i data/logo.mp4 -vcodec libx264 -preset medium -vprofile baseline -acodec aac -strict experimental -r 24 -b 255k -ar 44100 -s 1536x1152 -ab 59k -y 01_hd/misc/logo.mp4
avconv -i data/logo.mp4 -vcodec libx264 -preset medium -vprofile baseline -acodec aac -strict experimental -r 24 -b 255k -ar 44100 -s 1280x960 -ab 59k -y 02_threeQuarters/misc/logo.mp4
avconv -i data/logo.mp4 -vcodec libx264 -preset medium -vprofile baseline -acodec aac -strict experimental -r 24 -b 255k -ar 44100 -s 1280x960 -ab 59k -y 03_fifthEights/misc/logo.mp4
avconv -i data/logo.mp4 -vcodec libx264 -preset medium -vprofile baseline -acodec aac -strict experimental -r 24 -b 255k -ar 44100 -s 1024x768 -ab 59k -y 04_half/misc/logo.mp4


