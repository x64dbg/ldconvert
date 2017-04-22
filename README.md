# ldconvert

Library to convert 80-bit long double to and from strings.

**You need a custom mingw-w64 to compile a working version!** The patch is to change the implementation of `__fpclassifyl` with a call to `__builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, _x)` (both in `mingw-w64-headers/crt/math.h` and `mingw-w64-crt/math/fpclassifyl.c`).

You can compile mingw-w64 on lubuntu 16.04 x64 with the following commands:

```bash
sudo apt-get update
sudo apt-get install mingw-w64
wget https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/mingw-w64-v4.0.2.tar.bz2
tar -fuck I dont know tar commands (just extract the thing)
cd mingw-w64-v4.0.2
./configure --prefix=/usr/x86_64-w64-mingw32 --host=x86_64-w64-mingw32
make
sudo make install
./configure --prefix=/usr/i686-w64-mingw32 --host=i686-w64-mingw32
make
sudo make install
```

You can compile this on lubuntu 16.04 x64 with the following commands:

```bash
sudo apt-get update
sudo apt-get install git build-essential mingw-w64
make
```
