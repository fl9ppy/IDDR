import serial
import time

PORT = "/dev/ttyUSB0"   # Change if needed
BAUD = 115200

def open_serial():
    while True:
        try:
            print(f"Trying to connect to {PORT}...")
            ser = serial.Serial(PORT, BAUD, timeout=1)
            print("Connected.")
            return ser
        except serial.SerialException:
            print("ESP32 not found. Retrying in 2 seconds...")
            time.sleep(2)

def main():
    ser = open_serial()
    
    while True:
        try:
            line = ser.readline().decode("utf-8").strip()

            if line == "":
                continue  # ignore empty reads

            if line not in ["0", "1"]:
                print(f"Ignoring invalid data: {line}")
                continue

            # Convert to integer
            value = int(line)

            # Output or use the button state
            print(f"Button state: {value}")

        except serial.SerialException:
            print("Serial connection lost! Reconnecting...")
            ser = open_serial()

        except UnicodeDecodeError:
            print("Received non-UTF8 garbage, ignoring...")

if __name__ == "__main__":
    main()
