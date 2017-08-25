#!/bin/sh
export PATH=$PATH:/usr/local/bin
pm2 describe webapp &>/dev/null
if [[ $? -eq 0 ]]; then
    exit 0
else
    exit 1
fi