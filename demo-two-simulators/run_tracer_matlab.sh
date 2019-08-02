#!/bin/sh
rm *.log
rm *.out
export FNCS_LOG_STDOUT=no
export FNCS_LOG_FILE=yes
export FNCS_TRACE=yes
# matlab命令的位置
export matlab=/Applications/MATLAB_R2018a.app/bin/matlab

fncs_broker 3 > broker.out &
FNCS_CONFIG_FILE=simB.zpl ./simB > simB.out &
FNCS_CONFIG_FILE=tracer.zpl fncs_tracer 100s tracer.txt > tracer.out &
# matlab命令后的&符号会stop住进程，应该要放在最后一行
FNCS_CONFIG_FILE=simA.zpl $matlab -nodesktop -nojvm -nosplash -r "simA; exit;"
# 去掉官方demo里的wait,否则会hang住