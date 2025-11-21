from time import sleep
from Source_HV import src_hv
from VImeter import vi_meter


HV_src = src_hv.HV_source("/dev/ttyUSB0")

V1 = vi_meter.VI_meter("AKIP,AKIP-2101/2,NDM36GBD4R0067,3.01.01.07")
A1 = vi_meter.VI_meter("AKIP,AKIP-2101/2,NDM36GBQ4R0035,3.01.01.07")

file_name = "./data.txt"


def stepToCur(cur_target, save = False, file = None):

    if save == True and file == None:
        print("No enter file")
        return

    volt_src, cur_src = HV_src.read()

    print(f"Step to {cur_target} uA")
    print(f"Current V: {volt_src} V, I: {cur_src} uA")

    v = volt_src

    if cur_target < cur_src:
        k = -50
    else:
        k = 50
    
    cur_prev = HV_src.read()[1]

    while (cur_target < cur_src) == (k < 0):
        v += k
        HV_src.set(v, 5000)
        sleep(0.2)
        volt_src, cur_src = HV_src.read()

        if abs(cur_src-cur_prev) > 400:
            cur_tmp = 0
            for i in range(10):
                cur_tmp += HV_src.read()[1]
            cur_src = cur_tmp/10

        cur_prev = cur_src
        print(f"V: {volt_src} V, I: {cur_src} uA")

        if save == True:
            V1.writeCommand("READ?")
            A1.writeCommand("READ?")
            volt_rd = V1.readCommand()*10
            cur_rd = A1.readCommand()*1000
            print(f"read V: {volt_rd} V, I: {cur_rd} mA")
            file.write(f"{volt_rd}\t{cur_rd}\n")

        print("\n")

    print("End step")

def main():
    try:
        file = open(file_name, "w")

        if src_hv.checkPorts() or vi_meter.checkMeters():
            exit(1)

        if HV_src.init() or V1.init("V") or A1.init("A"):
            exit(1)

        input("Start?")


        volt_ign, cur_ign = 0, 0
        volt_src, cur_src = 0, 0
        cur_prev = 0

        for v in range(1000, 2301, 20):
            HV_src.set(v, 5000)
            print(f"Set: {v} V, 5 mA")

            sleep(0.2)

            volt_src, cur_src = HV_src.read()
            if abs(cur_src-cur_prev) > 400:
                cur_tmp = 0
                sleep(1)
                for i in range(10):
                    cur_tmp += HV_src.read()[1]
                cur_src = cur_tmp/10
                volt_src = HV_src.read()[0]
            
            cur_prev = cur_src
            print(f"source V: {volt_src} V, I: {cur_src} uA")
            print("\n")

            if cur_src > 1000:
                print("--Fire--")
                if volt_ign == 0:
                    volt_ign, cur_ign = volt_src, cur_src
                    break

        
        print(f"Ignition: {volt_ign} V, {cur_ign} uA")
        with open("ignition.txt", "w") as f:
            f.write(f"{volt_ign} {cur_ign}\n")

        stepToCur(600)
        stepToCur(4500, save=True, file=file)

    except Exception as e:
        print(f"Общая ошибка: {e}")
        
    finally:
        file.close()
        HV_src.deInit()
        V1.reset()
        A1.reset()


if __name__ == "__main__":
    main()
