from time import sleep
from Source_HV import src_hv
from VImeter import vi_meter


HV_src = src_hv.HV_source("/dev/ttyUSB0")

V1 = vi_meter.VI_meter("AKIP,AKIP-2101/2,NDM36GBD4R0063,3.01.01.07")
A1 = vi_meter.VI_meter("AKIP,AKIP-2101/2,NDM36GBD4R0068,3.01.01.07")

file_name = "./data.txt"

def found_ignition() :
    volt_ign, cur_ign = 0, 0
    volt_src, cur_src = 0, 0
    cur_prev = 0

    volt_ign, cur_ign = HV_src.stepToCur(300)

    print(f"Ignition: {volt_ign} V, {cur_ign} uA")
    with open("ignition.txt", "w") as f:
        f.write(f"{volt_ign} {cur_ign}\n")

def main():
    try:
        file = open(file_name, "w")

        if src_hv.checkPorts() or vi_meter.checkMeters():
            exit(1)

        if HV_src.init() or V1.init("V") or A1.init("A"):
            print("Init error")
            exit(1)

        input("Start?")


        

        HV_src.stepToCur(500)
        HV_src.stepToCur(4800, file=file, V1=V1, A1=A1)

    except Exception as e:
        print(f"Общая ошибка: {e}")
        
    finally:
        file.close()
        # HV_src.deInit()
        # V1.reset()
        # A1.reset()


if __name__ == "__main__":
    main()
