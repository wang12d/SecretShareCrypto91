#!/bin/bash

kSize=(10 20 30 40 50)

for i in "${kSize[@]}"
do
    for j in {1..2}
    do
        ./main $i >> exp.txt
    done
done

python3 dataHandler.py