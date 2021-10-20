#!/bin/bash
set -e
version=$(git tag | grep v | tail -n 1)
prefix=/usr

if [ -d deb_build ];
then
  rm -rf deb_build
fi

for arch in arm-linux-gnueabi x86_64-linux-gnu
do
  echo "Building $version for $arch"
  BUILDDIR=piaq-$version\_$arch

  # Make debian control files
  mkdir -p deb_build/$BUILDDIR/DEBIAN
  cp deb-control/control-$arch deb_build/$BUILDDIR/DEBIAN/control
  cp deb-control/pre* deb_build/$BUILDDIR/DEBIAN/
  chmod +x deb_build/$BUILDDIR/DEBIAN/pre*
  cp deb-control/post* deb_build/$BUILDDIR/DEBIAN/
  chmod +x deb_build/$BUILDDIR/DEBIAN/post*

  # Make fake root file system
  mkdir -p deb_build/$BUILDDIR$prefix/bin
  mkdir -p deb_build/$BUILDDIR$prefix/share/man/man1
  mkdir -p deb_build/$BUILDDIR/etc/piaq
  mkdir -p deb_build/$BUILDDIR/etc/systemd/system

  # Build and install files to fake root
  make install ARCH=$arch prefix=$prefix DESTDIR=deb_build/$BUILDDIR

  # Touch up a few things on the docs, which were installed by above command
  sed -e "s/piaq 0.0/piaq $version/g" -i deb_build/$BUILDDIR$prefix/share/man/man1/piaq.1
  sed -e "s/Version: 0.0/Version: $version/g" -i deb_build/$BUILDDIR/DEBIAN/control
  sed -e "s/Version: v/Version: /g" -i deb_build/$BUILDDIR/DEBIAN/control

  # Echo them for fun
  cat deb_build/$BUILDDIR/DEBIAN/control

  # Package it up
  pushd deb_build
  dpkg-deb --build --root-owner-group $BUILDDIR
  popd

done

mv deb_build/*.deb deb/
