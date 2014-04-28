#!/usr/local/bin/pyhton
import sys 

filename = sys.argv[1]

total_current = 0
i = 0

for line in open(filename):
    time, current = line.split(" ")

    total_current += float(current)
    i += 1

print total_current / i
