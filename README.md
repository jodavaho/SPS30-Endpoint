# What is this

A systemd-integrated, openmetrics-spewing utility for a rasp pi that has an SPS30 connected. Nice for [prometheus](https://prometheus.io) monitoring.

# Binary requires 

- Nothing 

you *do* need to have your sps30 set up on your pi / device, and you *do* need to have a linux driver working. See [sps 30 driver notes, below](#sps30-driver)

## Installation on Raspberry Pi

1. just download `deb/piaq-v0.9.0_arm-linux-gnueabi.deb` to your pi and `dpkg -i piaq-v0.9.0_arm-linux-gnueabi.deb`. 
2. From any machine that's networked, or the pi itself, `curl <pi's IP>:8051` to see your values. The server will restart automatically using systemd, so you're off and running!

Configure `/etc/piaq/piaq.conf` till your heart's content to change port, the files to query, and the time to sleep between queries.

# Roadmap

See issues. 

- cli args

See [Installing Binary](#installing-binary-only)

# Build Requires

Try: `fetch_deps.sh` on ubuntu

Otherwise:

- libconfig-dev
- make (gnu)
- g++
- libasio-dev

# Building & Installing from Source

Build for local machine:

```
make
make install
```

In general:

```
make ARCH=<arch triplet> 
make install prefix=<optional prefix>
```
By default, `ARCH=$(gcc -dumpmachine)`. You can use this to cross compile. 

Example, cross compile for arm:

```
make ARCH=arm-linux-gnueabi
```

# Installing binary only

Just grab a deb in deb/
```
dpkg -i deb/piaq-<version>-<arch>.deb
```

# SPS30 Driver

This is not hard, and is well covered by [these directions by sensirion](https://github.com/Sensirion/linux-sps30) but generally looks something like this:

```
# Load i2c linux driver and crc8 code
sudo modprobe industrialio
sudo modprobe crc8
# initialize sensirion's excellent driver
sudo modprobe sps30
# tell the kernel to find a device at address 0x68 and to use sps30 as its driver
echo sps30 0x69 | sudo tee /sys/class/i2c-adapter/i2c-1/new_device
```
