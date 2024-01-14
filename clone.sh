#! /bin/sh

pushd external

if [[ -e fldigi ]]; then
    pushd fldigi
    git pull
    popd
else
    git clone https://github.com/w1hkj/fldigi.git
fi

if [[ -e hamlib ]]; then
    pushd hamlib
    git pull
    popd
else
    git clone https://github.com/Hamlib/Hamlib.git hamlib
fi

popd
