import sys, math
from fpbinary import FpBinary
import numpy as np

def main():
    if len(sys.argv) != 2:
        print("need an argument (a quoted string representing the math function you want a LUT for)")
        return
    func = sys.argv[1]
    x = FpBinary(int_bits=16, frac_bits=16, signed=False, bit_field=0)
    iter = FpBinary(int_bits=16, frac_bits=16, signed=False, bit_field=0x100)
    try:
        f = open("lut.c", "w")
    except:
        print("lut.c already exists. Exiting")
        return
    with open("lut.c", "a") as f:
        f.write("const u16 lut[] = {\n")
        while x < 1:
            y = FpBinary(format_inst=x, value=eval(func))
            f.write("    0b" + np.binary_repr(y) + ",\n")
            x += iter
        f.write("};\n")

main()
