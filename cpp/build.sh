#!/bin/bash
mkdir include
mkdir build
cd src
qmake src.pro
make
make install
cp *.h ../include
