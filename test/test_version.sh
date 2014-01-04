#!/bin/bash

if [ -x ../stonesh ]; then
    ../stonesh -v
    exit 0
else
    echo "####"
    echo "#### stonesh is missing, you should run make in the top level directory first"
    echo "####"
    exit 1
fi
