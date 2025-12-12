#!/bin/bash

if [ $# != 2 ] && [ $# != 3 ] ; then

    echo "Usage: <file> <fileres> <mode (onlyres)>"

else


    file=$1
    fileres=$2
    mode=""
    if [ $# == 3 ]; then
	    mode=$3
    fi

    # filenamelen=${#file}

    filewords="$(wc -w $file)"
    filewords="$(echo $filewords | cut -d' ' -f1)"

    # echo $filewords
    echo $mode
    echo $#

    echo > $fileres

    if [ $# == 3 ] && [ "$mode" = "onlyres" ];then
    	for ((i = 1 ; i < $filewords ;i++)); do
        	# echo "====================================">> $fileres
        	./a.out $i $filewords $file | grep RES >> $fileres 2>&1
        	# echo "====================================">> $fileres
            done
    else
	for ((i = 1 ; i < $filewords ;i++)); do
        	echo "====================================">> $fileres
        	./a.out $i $filewords $file >> $fileres 2>&1
        	echo "====================================">> $fileres
            done
    fi
fi
