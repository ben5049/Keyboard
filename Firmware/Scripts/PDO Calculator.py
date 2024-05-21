
def pdo_decode(pdo):
    print(pdo,":",sep="")
    
    pdo = str(bin(int(pdo.split("x", 1)[-1], 16))).split("b", 1)[-1].zfill(32)
    
    print("Voltage =", int(pdo[-20:-10],2) * 50, "mV")
    print("Current =", int(pdo[-10:],2) * 10, "mA")

    print("Fast role swap =", end=" ")
    if pdo[-25:-23] == "00":
        print("not supported")
    elif pdo[-25:-23] == "11":
        print("3A at 5V")
    elif pdo[-25:-23] == "10":
        print("1.5A at 5V")
    else:
        print("default USB power")
    
    print("Dual-Role Data =", pdo[-26] == "1")
    print("USB Communication capable =", pdo[-27] == "1")
    print("Unconstrained power =", pdo[-28] == "1")
    print("Higher capability =", pdo[-29] == "1")
    print("Dual-Role power =", pdo[-30] == "1")
    print("Fixed supply =", pdo[-32:31] == "00")

    print()
    

    

pdo_decode("0x2201912C")
pdo_decode("0x26019096")
pdo_decode("0x02019096")
