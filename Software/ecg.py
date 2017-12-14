# -*- coding: utf-8 -*-
"""
Author: Erhan YILMAZ
Date: 14.12.17
Description: Python software to monitoring ECG signal on the computer.
"""

import numpy as np
import matplotlib.pyplot as plt
from random import *
import serial



class RingBuffer:
    def __init__(self, size):
        self.data = [None for i in range(size)]

    def append(self, x):
        self.data.pop(0)
        self.data.append(x)

    def get(self):
        return self.data


buf = RingBuffer(100)
for i in range(100):
        buf.append(randint(0,4))

x = np.arange(1,101)
plt.ion()
fig = plt.figure()


ax = fig.add_subplot(111)
line1, = ax.plot(x, buf.get(), 'b-')
#fig.canvas.draw()
ax.set_title("ECG")
ax.set_xlabel("Samples[n]")
ax.set_ylabel("Voltage")

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
	port='COM8',
	baudrate=9600,
	parity=serial.PARITY_ODD,
	stopbits=serial.STOPBITS_TWO,
	bytesize=serial.SEVENBITS
)
#ser.close()


if ser.is_open==False:
    ser.open()


while 1:
    while ser.inWaiting() > 0:
        #print(ord(ser.read(1))/127*3.3)
        buf.append(ord(ser.read(1))/127*3.3)
        line1.set_ydata(buf.get())
        plt.pause(0.001)
        #fig.canvas.draw()
        #time.sleep(0.5)
        #print(buf.get())



    
