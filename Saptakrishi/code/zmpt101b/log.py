import serial
import csv

def read_serial_to_csv(port, baudrate, csv_file):
    try:
        # Open the serial port
        ser = serial.Serial(port, baudrate, timeout=1)
        print(f"Connected to {port} at {baudrate} baud.")

        with open(csv_file, 'w', newline='') as file:
            writer = csv.writer(file)
            # Write CSV header
            writer.writerow(['Time', 'Value'])

            print("Reading data...")
            while True:
                if ser.in_waiting > 0:
                    # Read line from serial port
                    line = ser.readline().decode('utf-8').strip()
                    
                    if line:  # Make sure line is not empty
                        try:
                            # Separate time and value
                            time, value = line.split()
                            writer.writerow([time, value])
                        except ValueError:
                            print(f"Skipping line due to format error: {line}")
    except serial.SerialException as e:
        print(f"Error: {e}")
    finally:
        if ser.is_open:
            ser.close()
            print("Serial port closed.")

if __name__ == "__main__":
    port = 'COM17'  # Replace with your serial port
    baudrate = 9600  # Replace with your baud rate
    csv_file = 'serial_data.csv'  # Output CSV file

    read_serial_to_csv(port, baudrate, csv_file)
