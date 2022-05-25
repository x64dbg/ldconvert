#!/usr/bin/env python3

import pefile
import sys
import struct

def main():
    if len(sys.argv) < 2:
        print("Usage: patch-tls.py ldconvert.dll")
        exit(1)

    file = sys.argv[1]
    pe = pefile.PE(file)
    if pe.FILE_HEADER.Machine == 0x8664:
        ptrsize = 8
        format = "<Q"
    else:
        ptrsize = 4
        format = "<L"

    # Set AddressOfCallBacks to an empty array
    tls = pe.DIRECTORY_ENTRY_TLS
    offset = tls.struct.get_field_absolute_offset("AddressOfCallBacks")
    oldvalue = tls.struct.AddressOfCallBacks
    newvalue = oldvalue - ptrsize

    with open(file, "r+b") as f:
        f.seek(offset)
        f.write(struct.pack(format, newvalue))
    
    print(f"Patched AddressOfCallBacks (offset 0x{offset:x}) from 0x{oldvalue:x} to 0x{newvalue:x}")


if __name__ == "__main__":
    main()
