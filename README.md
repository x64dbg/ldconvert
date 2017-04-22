# ldconvert

Library to convert 80-bit long double to and from strings.

You can compile this on lubuntu 16.04 x64 with the following commands:

```bash
sudo apt-get update
sudo apt-get install git build-essential mingw-w64
make
tar -zcvf ldconvert.tar.gz bin
curl --upload-file ldconvert.tar.gz https://transfer.sh/ldconvert.tar.gz && echo
```
