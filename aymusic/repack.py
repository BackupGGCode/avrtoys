#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os

def split_len(seq, length):
    return [seq[i:i+length] for i in range(0, len(seq), length)]

f = open(sys.argv[1], "rb").read()
sframes = len(f)/14

frames = split_len(f, sframes)

of = open(sys.argv[2], "wb")

for i in xrange(0, sframes):
    for j in xrange(0, 14):
        of.write(frames[j][i])

print "Total steps ", sframes

"""
cnt = 0
s = ''
for j in xrange(0, 14):
    s += hex(ord(frames[j][0]))+", "
print s 

for i in xrange(1, 1400):
    s = ''
    for j in [0,1, 2,3, 4,5, 6, 8,9,10, 7]:
        if frames[j][i] != frames[j][i-1]:
            s += hex(j)+","+hex(ord(frames[j][i]))+", "
            cnt += 2
    s += "0xFF, " 
    cnt += 1
    print s

print "0xFE // Total "+str(cnt+1)+" bytes"
"""
