#!/bin/sh

#create files
touch \ ABC\ DEF
touch ./-AbC\ aBc
touch ./--ABC\ DEF

ls

find -type f |
while read -r file; do
  newname=$(echo "$file" | tr '[:upper:]' '[:lower:]')
  mv "$file" "$newname"
done

echo "________________________"
ls

#remove files
rm \ abc\ def
rm ./-abc\ abc
rm ./--abc\ def

echo "________________________"
ls