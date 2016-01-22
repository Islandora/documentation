#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
for example_file in $DIR/example.*; do
    filename=$(basename "$example_file")
    filename=${filename#example\.};
    if [ ! -f $DIR/$filename ]; then
        cp $example_file $DIR/$filename
    fi
done
