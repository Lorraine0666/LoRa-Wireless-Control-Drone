import serial

# get input message for baudrate
# just a helper function 
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

# * 1. get serial port information from terminal
port = input("Please enter the serial port (e.g., /dev/ttyUSB0 or COMx): ")
baudrate = get_input("Please input baudrate (e.g., 9600): ", int, valid_range=range(1, 1000000))

# * 2. open serial
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

# * 3. send AT command to set serial
if ser.is_open:
    # TODO: send AT command according to HC-T串口助手
    # ! need to change the following lines
    ser.close()
    print(f"Serial port {port} closed.")


while True:
    try:
        # * 4. receive command from terminal
        yaw = float(input("Please input yaw value: "))
        pitch = float(input("Please input pitch value: "))
        roll = float(input("Please input roll value: "))

        # * 5. concatenate string
        command = f"<y:{yaw},p:{pitch},r:{roll}>"
        print(f"Sending command: {command}")

        # * 6. send message 
        ser.write(command.encode())

    except ValueError:
        print("Invalid input!")
    except KeyboardInterrupt:
        print("Abort!")
        break 
    except Exception as e:
        print(f"Error happen: {e}")
        break 

ser.close()
