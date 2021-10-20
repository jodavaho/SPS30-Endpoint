mkdir -p deps
sudo apt install libasio-dev
sudo apt install libconfig-dev
git clone https://gitlab.com/eidheim/Simple-Web-Server deps/SimpleWebServer
git clone https://github.com/hyperrealm/libconfig deps/libconfig
echo "Building dependencies ... "


echo "Building libconfig"
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

