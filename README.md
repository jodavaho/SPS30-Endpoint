# Binary requires

- None

# Build Requires

- GNU Make
- GCC/G++
- libasio-dev

# Building & Installing from Source

```
make ARCH=<arch triplet> 
make install prefix=<optional prefix>
```

For example:

```
make ARCH=x86_64-linux-gnu
make install prefix=/usr/local
```

By default, `ARCH=$(gcc -dumpmachine)`

# Installing binary only

```
dpkg -i deb/piaq-v1.0.deb
```
