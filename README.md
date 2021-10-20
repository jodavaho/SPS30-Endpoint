# Binary requires 

- None, just download `deb/piaq-v0.9.0_arm-linux-gnueabi.deb` to your pi and `dpkg -i piaq-v0.9.0_arm-linux-gnueabi.deb`. 

Then, run `piaq`

From any machine that's networked, or the pi itself, `curl <pi's IP>:8051` to see your values.

Configure `/etc/piaq/piaq.conf` till your heart's content to change port, the files to query, and the time to sleep between queries.

# Roadmap

See issues. 

- Systemd integration
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
