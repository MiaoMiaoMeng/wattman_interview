#!/bin/bash

fileCount=0

function foreachd(){
for  file  in  $1/*
    do
        if [[ "$file" == *_gt.json ]]; then
            let fileCount+=1
        elif [[ -d "$file" ]]; then
            foreachd $file
        fi
    done
}

if [[ "x$1" == 'x' ]]
then
    foreachd "."
else
    foreachd "$1"
fi 

echo  "Function1: There are $fileCount files in $PWD"
echo  "Function2: There are `find $1 -name *'\'_gt.json* | wc -l` files"