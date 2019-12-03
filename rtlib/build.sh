# !/usr/bin/bash

mkdir build

pushd llmpxrt
make
cp libllmpx_rt.a ../build
popd 

pushd memptx
make
cp libinterceptor.a ../build
popd

pushd mpxrt
make
cp libmpxrt.a ../build
popd

pushd mpxwrap
make
cp libmpxwrappers.a ../build
popd

cd build
ar -x libllmpx_rt.a
ar -x libinterceptor.a
ar -x libmpxrt.a
ar -x libmpxwrappers.a
ar -rc libmpx.a *.o
cd ..

cp build/libmpx.a ../build/lib
