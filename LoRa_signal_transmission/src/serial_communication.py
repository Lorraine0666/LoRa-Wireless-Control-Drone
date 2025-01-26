import serial
import time

def send_receive_data(port, baud_rate):
    # 尝试打开串口
    try:
        ser = serial.Serial(port, baud_rate, timeout=1)
        print(f"串口 {port} 已打开，波特率 {baud_rate}")

        # 发送数据
        while True:
            to_send = input("请输入要发送的数据: ")
            ser.write(to_send.encode())
            print(f"发送: {to_send}")

            # 等待接收数据
            if ser.in_waiting > 0:
                received_data = ser.readline().decode().strip()
                print(f"接收: {received_data}")
            time.sleep(1)

    except serial.SerialException as e:
        print(f"无法打开串口 {port}: {e}")
    finally:
        if ser.is_open:
            ser.close()
        print("串口已关闭")

if __name__ == "__main__":
    PORT = 'COM4'  # 端口号
    BAUD_RATE = 115200   # 波特率
    send_receive_data(PORT, BAUD_RATE)