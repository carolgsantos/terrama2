#!/bin/sh
pm2 stop webapp webmonitor | pm2 save
if [[ $? -eq 0 ]]; then
    exit 0
else
    exit 1
fi