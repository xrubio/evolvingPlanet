#!/bin/bash
avconv -i data/logo.mp4 -profile:v baseline -level 30 -s 2048x1536 -c:v h264 -c:a copy  -acodec libmp3lame -ac 2 01_hd/misc/logo.mp4
avconv -i data/logo.mp4 -profile:v baseline -level 30 -s 1536x1152 -c:v h264 -c:a copy  -acodec libmp3lame -ac 2 02_threeQuarters/misc/logo.mp4
avconv -i data/logo.mp4 -profile:v baseline -level 30 -s 1280x960 -c:v h264 -c:a copy  -acodec libmp3lame -ac 2 03_fifthEights/misc/logo.mp4
avconv -i data/logo.mp4 -profile:v baseline -level 30 -s 1024x768 -c:v h264 -c:a copy  -acodec libmp3lame -ac 2 04_half/misc/logo.mp4

