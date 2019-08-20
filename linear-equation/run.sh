#!/bin/sh
rm *.log
rm *.out
export FNCS_LOG_STDOUT=no
export FNCS_LOG_FILE=yes
export FNCS_TRACE=yes
fncs_broker 3 > broker.out &
FNCS_CONFIG_FILE=simA.zpl ./simA > simA.out &
FNCS_CONFIG_FILE=simB.zpl ./simB > simB.out &
FNCS_CONFIG_FILE=simC.zpl ./simC > simC.out
