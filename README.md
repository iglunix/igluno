# igluno - Our own fork of pico 
### How to compile
Download alpine.tar.gz from `http://ftp.swin.edu.au/alpine/`. Extract it. Empty the pico folder and put the content of this repo in it. At the ´alpine-2.00` folder, run `./configure`. After that run `make -s c-client` then `make -s c-client.d` and then `make -s -C pith`. When those are done, `cd` into the pico folder and run `make`. This will create the igluno excecutabe named `pico`. To run it, use `./pico` inside the pico folder.
