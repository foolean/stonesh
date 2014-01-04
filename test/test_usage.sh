#!/bin/bash

if [ -x ../stonesh ]; then
    ../stonesh -h
    exit 0
else
    echo "####"
    echo "#### stonesh is missing, run make in the top level directory first"
    echo "####"
    exit 1
fi
