# /bin/bash

export OPENBLAS_NUM_THREADS=2
export BYPASSACL=0xff7c
taskset -a 0x30 ./build/bin/face-recognition
