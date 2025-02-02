import serial

def get_input(prompt, cast_type, valid_range=None):
    """Helper function to get valid input from the user."""
    while True:
        try:
            user_input = cast_type(input(prompt))  
            if valid_range and user_input not in valid_range:
                print(f"Invalid input. Please enter a value within {valid_range}.")
                continue
            return user_input
        except ValueError:
            print("Invalid number, please enter a valid value.")


port = input("Please enter the serial port (e.g., /dev/ttyUSB0 or COMx): ")
baudrate = get_input("Please input baudrate (e.g., 9600): ", int, valid_range=range(1, 1000000))


roll = get_input("Please input roll: ", float)
pitch = get_input("Please input pitch: ", float)
yaw = get_input("Please input yaw: ", float)


try:
    ser = serial.Serial(port=port, 
                        baudrate=baudrate,
                        bytesize=8,
                        parity=serial.PARITY_NONE, 
                        stopbits=serial.STOPBITS_ONE, 
                        timeout=1)
    print(f"Serial port {port} opened successfully.")
except serial.SerialException as e:
    print(f"Failed to open serial port {port}: {e}")


if ser.is_open:
    ser.close()
    print(f"Serial port {port} closed.")
