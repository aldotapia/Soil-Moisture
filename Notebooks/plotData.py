import serial
from matplotlib import pyplot as plt

port = '/dev/cu.usbmodem143401'
sensor = 'ComWinTop Soil Moisture Sensor (Model THC-S)'

# begin serial communication
ser = serial.Serial(port, 9600)

# remove all serial data from the buffer before reading
ser.reset_input_buffer()

# read data from serial
data = ser.readline().decode().strip()

# wait until the first line of  valid data is received
while len(data.split('. ')) != 3:
    data = ser.readline().decode().strip()

# Initialize lists to store data for three different lines
data_line1 = []
data_line2 = []
data_line3 = []

# Function to update the plot with data from the serial port
while True:
    try:
        # Read data from serial
        data = ser.readline().decode().strip()

        # wait until the first line of  valid data is received
        while len(data.split('. ')) != 3:
            data = ser.readline().decode().strip()

        # get data from the serial output
        t = float(data.split('. ')[0].split(' ')[1])
        h = float(data.split('. ')[1].split(' ')[1])
        c = float(data.split('. ')[2].split(' ')[1])
        # append data to the lists
        data_line1.append(t)
        data_line2.append(h)
        data_line3.append(c)

        # plot
        # create a plot with two y axis
        # first y axis for temperature and humidity
        fig, ax = plt.subplots()
        line1, = ax.plot(data_line1, color = 'b',label='Temperatura [ºC]')
        line2, = ax.plot(data_line2, color = 'g', label='Humedad [%]')
        # second y axis for conductivity
        ax2 = ax.twinx()
        line3, = ax2.plot(data_line3, color='r', label='Conductividad [µS/cm]')
        # mix legend of ax and ax2
        lines = [line1, line2, line3]
        labels = [l.get_label() for l in lines]
        ax.legend(lines, labels, loc=0)

        # Set up the plot's properties
        # update the x axis limits based on the number of data points
        ax.set_xlim(0, len(data_line1))
        ax.set_ylim(0, 100)
        ax2.set_ylim(0, 800)

        # set labels
        ax.set_xlabel('Observaciones')
        ax.set_ylabel('Temperatura [ºC] / Humedad [%]')
        ax2.set_ylabel('Conductividad [µS/cm]')
        ax.set_title(f'Datos de sensor {sensor}')
        plt.pause(0.05)

    except KeyboardInterrupt:
        ser.close()