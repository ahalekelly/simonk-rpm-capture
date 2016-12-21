import serial
from time import sleep
import csv
import re

port = 'COM5'
fileName = 'outrunner-NTM-3530-1400.csv'

ser = serial.Serial(port, 115200, timeout=0)
times = []
speeds = []
data = ''

loops = 10

ser.write((str(loops)+'\n').encode('ascii'));
while 's' not in data:
    data += ser.read(1000000).decode('ascii')
    sleep(0.1)

with open(fileName, 'a') as csvFile:
    csvWriter = csv.writer(csvFile)
    for timeMicros, commutationMicros in re.findall('b(\d*)m(\d*)e',data):
        csvWriter.writerow((int(timeMicros)/1000000, 60/2*1000000/int(commutationMicros)))

print('done')