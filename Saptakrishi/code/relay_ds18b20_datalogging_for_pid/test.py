import serial
import csv

# Define the COM port and baud rate
port = 'COM17'  # Change this to the correct COM port for your ESP32
baud_rate = 115200

# Open the serial connection
ser = serial.Serial(port, baud_rate)

# Create a CSV file and write headers
csv_filename = 'esp32_data_log.csv'
csv_file = open(csv_filename, 'w', newline='')
csv_writer = csv.writer(csv_file)
csv_writer.writerow(['time', 'temperature', 'relay_state'])  # CSV headers

try:
    while True:
        # Read data from the serial port
        data = ser.readline().decode().strip()  # Decode and strip any whitespace

        # Print the data to the console
        print("Received data:", data)

        # Split the data assuming it's comma-separated (time, temperature, relay_state)
        data_parts = data.split(',')

        # Ensure correct number of data parts before writing to the CSV file
        if len(data_parts) == 3:
            csv_writer.writerow(data_parts)  # Write data to CSV file
        else:
            print("Malformed data received:", data)

except KeyboardInterrupt:
    print("Logging stopped by user.")

finally:
    # Close serial connection and CSV file
    ser.close()
    csv_file.close()
