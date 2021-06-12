# igluno - Our own fork of pico
### How to compile
```wget http://ftp.swin.edu.au/alpine/alpine.tar.gz
tar -xf alpine.tar.gz
cd alpine-2.00
git clone https://github.com/iglunix/igluno
rm -rf pico configure
cp ./igluno/configure ./
./configure
make -s c-client
make -s c-client.d
make -s -C pith
cd igluno
make```
