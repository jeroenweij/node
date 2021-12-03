#!/bin/bash

PIN_DEFINITION="$1"
PIN_HEADER="$2"
rm -rf $PIN_HEADER

echo "" >> $PIN_HEADER
echo "#pragma once" >> $PIN_HEADER
echo "" >> $PIN_HEADER

while read line; do 
    P="$(echo $line | cut -d , -f 1)"
    C="$(echo $line | cut -d , -f 2 | xargs)"
    N="$(echo $line | cut -d , -f 3 | xargs)"

    N="${N^^}"
    N="${N// /_}"
    if [ -n "$P" ] && [ -n "$N" ]; then
        if [ -n "$C" ]; then
            echo "#define PIN_$N $P // $C" >> $PIN_HEADER
        else
            echo "#define PIN_$N $P" >> $PIN_HEADER
        fi
    fi

done < ${PIN_DEFINITION} 
    

