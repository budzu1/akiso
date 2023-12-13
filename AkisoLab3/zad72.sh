#!/bin/sh

#create files
touch \ ABC\ DEF
touch ./-AbC\ aBc
touch ./--ABC\ DEF

ls

ls | xargs echo

echo "________________________"
ls

#remove files
rm \ abc\ def
rm ./-abc\ abc
rm ./--abc\ def

echo "________________________"
ls