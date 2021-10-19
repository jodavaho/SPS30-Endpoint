mkdir -p deps
git clone https://gitlab.com/eidheim/Simple-Web-Server deps/SimpleWebServer
# Needed for buliding SimpleWebServer in
sudo apt install libasio-dev
git clone https://github.com/hyperrealm/libconfig deps/libconfig
echo "Building libconfig once and for all ... "
cd deps/libconfig
for arch in arm-linux-gnueabi x86_64-linux-gnu
do
  if [ -d build-$arch ];
  then
    rm -rf build-$arch
  fi
  mkdir build-$arch
  pushd build-$arch 
  cmake .. -DCMAKE_C_COMPILER=$arch-gcc -DCMAKE_CXX_COMPILER=$arch-g++ -DBUILD_SHARED_LIBS=Off
  make 
  popd
done

