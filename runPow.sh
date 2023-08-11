#!/bin/bash

MY=../CPar-TP
SORT_SIZE=8000

export OMP_NUM_THREADS=16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16

export OMP_NUM_THREADS=32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32

export OMP_NUM_THREADS=64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64

SORT_SIZE=64000

export OMP_NUM_THREADS=16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16

export OMP_NUM_THREADS=32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32

export OMP_NUM_THREADS=64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64

SORT_SIZE=6400000

export OMP_NUM_THREADS=16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16

export OMP_NUM_THREADS=32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32

export OMP_NUM_THREADS=64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64

SORT_SIZE=2000000000

export OMP_NUM_THREADS=16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16
$MY/sort $SORT_SIZE 16

export OMP_NUM_THREADS=32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32
$MY/sort $SORT_SIZE 32

export OMP_NUM_THREADS=64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64
$MY/sort $SORT_SIZE 64