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
  mkdir -p deb_build/$BUILDDIR/DEBIAN
  cp deb/control-$arch deb_build/$BUILDDIR/DEBIAN/control
  mkdir -p deb_build/$BUILDDIR$prefix/bin
  mkdir -p deb_build/$BUILDDIR$prefix/share/man/man1
  mkdir -p deb_build/$BUILDDIR/etc/piaq
  mkdir -p deb_build/$BUILDDIR/etc/systemd/system
  make install ARCH=$arch prefix=$prefix DESTDIR=deb_build/$BUILDDIR
  sed -e "s/piaq 0.0/piaq $version/g" -i deb_build/$BUILDDIR$prefix/share/man/man1/piaq.1
  sed -e "s/Version: 0.0/Version: $version/g" -i deb_build/$BUILDDIR/DEBIAN/control
  sed -e "s/Version: v/Version: /g" -i deb_build/$BUILDDIR/DEBIAN/control
  cat deb_build/$BUILDDIR$prefix/share/man/man1/piaq.1
  cat deb_build/$BUILDDIR/DEBIAN/control
  pushd deb_build
  dpkg-deb --build --root-owner-group $BUILDDIR
  popd
done

mv deb_build/*.deb deb/
