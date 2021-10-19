#!/bin/bash
set -e
version=$(git tag | grep v | tail -n 1)
prefix=/usr

for arch in arm-linux-gnueabi x86_64-linux-gnu
do
  echo "Building $version for $arch"
  BUILDDIR=piaq-$version\_$arch
  mkdir -p deb_build/$BUILDDIR/DEBIAN
  cp deb/control-$arch deb_build/$BUILDDIR/DEBIAN/control
  mkdir -p deb_build/$BULIDDIR$prefix/bin
  mkdir -p deb_build/$BUILDDIR$prefix/share/man/man1
  mkdir -p deb_build/$BUILDDIR/etc
  make install ARCH=$arch prefix=$prefix DESTDIR=deb_build/$BUILDDIR
  pushd deb_build
  dpkg-deb --build --root-owner-group $BUILDDIR
  popd
done

mv deb_build/*.deb deb/
rm -rf deb_build
