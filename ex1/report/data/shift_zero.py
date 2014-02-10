import sys 

filename = sys.argv[1]

f = open(filename+"-new", "w")
initialSeconds=0
i = 0

for line in open(filename):
    if i == 0:
        i += 1
        continue

    time, current, voltage, pc, irq = line.split(" ; ")

    # Record time value of first data point
    if i == 1:
        i += 1
        initialSeconds = int(time)

    time = float(int(time) - int(initialSeconds)) / 1000000
    f.write(str(time) + ' ' + str(current) + '\n')
