# MachSuite

This repo is forked from [MachSuite](https://github.com/breagen/MachSuite) github. I have modified the Makefile to generate bitstream and host file for following benchmarks:
 - FFT/Strided
 - GEMM/ncubed
 - MD/KNN
 - NW/NW
 - Stencil/Stencil2D
 - Stencil/Stencil3D
 
They are tested on ZCU102 evaluation boards, and their results are correct.
### Installation
You need Vivado18.3 and SDSoC18.3 to synthesize and compile the benchmarks.
First source the Vivado and SDx setting files:
```sh
$ source /tools/Xilinx/Vivado/2018.3/settings64.sh
$ source /tools/Xilinx/SDx/2018.3/settings64.sh
$ export PATH=$PATH:/tools/Xilinx/Vivado/2018.3/bin:/opt/Xilinx/SDx/2018.3/bin/
$ export XILINX_VIVADO_HLS=/tools/Xilinx/Vivado/2018.3/
```
Then run the make command in the subdirectory of each of the aformentioned benchmarks:
```sh
$ cd /path/to/MachSuite/fft/strided
$ ./synth.sh
```

### Emulation
```sh
$ cd /path/to/MachSuite/fft/strided
$ make
```
It will automatically call qemu and start PL/AP emulation. If you need to see the waveform and data transaction, you need to run `make` command as follows:
```sh
$ cd /path/to/MachSuite/fft/strided
$ make EMU_MODE=debug
```

Enjoy!!
