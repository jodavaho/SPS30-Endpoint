# Binary requires 

- None

See [Installing Binary](Installing-binary-only)

# Build Requires

- GNU Make
- GCC/G++
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

```
dpkg -i deb/piaq-v1.0.deb
```
