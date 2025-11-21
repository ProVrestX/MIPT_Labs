from .comm import *
from time import sleep


command = {"change": [1], "enable": [2], "clear": [3], "read": [5]}
Kv = 10.67
Ki = 6.4


def calcVoltNum(volt):
    number = int(volt * Kv)
    num_bytes = number.to_bytes(2, byteorder='big', signed=True)
    return num_bytes

def calcCurrNum(curr):
    number = int(curr * Ki)
    num_bytes = number.to_bytes(2, byteorder='big', signed=True)
    return num_bytes

def getVIMsg(volt, curr):
    msg = command["change"][:]
    
    volt_bytes = calcVoltNum(volt)
    curr_bytes = calcCurrNum(curr)
    
    msg.append(volt_bytes[1])  # Младший байт напряжения
    msg.append(volt_bytes[0])  # Старший байт напряжения
    msg.append(curr_bytes[1])  # Младший байт тока
    msg.append(curr_bytes[0])  # Старший байт тока
    
    return msg


class HV_source:
    def __init__(self, port):
        self.port = port
        self.ser = Serial(port, 38400)

    ser = None

    def init(self):
        if not (self.port in listAvailablePorts()):
            print(f"COM-порт {self.port} не найден")
            return 1

        if self.ser.open() == False:
            return 1
        
        return 0

    def set(self, volt, curr):
        if not self.ser.checkPortState():
            return 1
        
        msg = getVIMsg(volt, curr)
        # print(f"Отправка сообщения: {msg}")
        self.ser.write(msg)
        self.ser.write(command["enable"])

        return 0
    
    def deInit(self):
        if not self.ser.checkPortState():
            return 1
        
        self.ser.write(command["clear"])
        self.ser.close()

        return 0

    def read(self):
        if not self.ser.checkPortState():
            return None

        self.ser.write(command["read"])
        data = self.ser.read(5)

        if data:
            data_list = [hex(byte) for byte in data]
            # print(f"Полученные данные: {data_list}")
        else:
            print("Не удалось получить данные")
        
        V = int(data_list[2][2:] + data_list[3][2:], 16)
        V //= Kv
        I = int(data_list[0][2:] + data_list[1][2:], 16)
        I //= Ki

        return [V, I]


