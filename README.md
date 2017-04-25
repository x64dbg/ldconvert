# ldconvert

Library to convert 80-bit long double to and from strings.

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
