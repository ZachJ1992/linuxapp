#!/bin/sh

rm ~/nfsroot/main

echo "first ls ~/nfsroot/ :"
ls ~/nfsroot/ 

echo "copy app to nfsroot"
cp ./main ~/nfsroot/

echo "second ls ~/nfsroot/ :"
ls ~/nfsroot/ 
