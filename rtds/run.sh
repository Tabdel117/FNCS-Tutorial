#!/bin/sh
rm *.log
rm *.out
export FNCS_LOG_STDOUT=no
export FNCS_LOG_FILE=yes
export FNCS_TRACE=yes
fncs_broker 2 > broker.out &
FNCS_CONFIG_FILE=server.zpl ./server > server.out &
FNCS_CONFIG_FILE=client.zpl ./client > client.out
