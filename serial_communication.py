import serial
import time

def send_receive_data(port, baud_rate):
    try:
        ser = serial.Serial(port, baud_rate, timeout=1)
        print(f"串口 {port} 已打开，波特率 {baud_rate}")

        while True:
            command = input("请输入指令 (A 或 S): ")
            if command in ['A', 'S']:
                ser.write(command.encode())  # 发送指令
                print(f"发送: {command}")

                # 等待接收确认消息
                if ser.in_waiting > 0:
                    response = ser.readline().decode().strip()
                    print(f"接收: {response}")
                else:
                    print("未收到确认消息")
            else:
                print("无效指令，请输入 A 或 S")
            time.sleep(1)

    except serial.SerialException as e:
        print(f"无法打开串口 {port}: {e}")
    finally:
        if ser.is_open:
            ser.close()
        print("串口已关闭")

if __name__ == "__main__":
    PORT = 'COM4'  # 替换为你的串口端口号
    BAUD_RATE = 115200  # HC-14 的波特率
    send_receive_data(PORT, BAUD_RATE)