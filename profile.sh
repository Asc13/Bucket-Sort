#!/bin/bash

MY=../CPar-TP

export OMP_NUM_THREADS=16
perf record  $MY/sort 1000000 16
perf report -n > perfreport