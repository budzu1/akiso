#!/bin/sh

#create files
touch \ ABC\ DEF
touch ./-AbC\ aBc
touch ./--ABC_DEFG

ls

find -type f |
while read -r file; do
  newname=$(echo "$file" | tr -d '_' | tr -d '-' |tr '[:upper:]' '[:lower:]')
  mv "$file" "$newname"
done

echo "________________________"
ls

#remove files
rm \ abc\ def
rm ./abc\ abc
rm ./abcdefg

echo "________________________"
ls
