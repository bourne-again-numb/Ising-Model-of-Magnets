#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: ./make.sh <model_type> (e.g. ising, epidemic_hs)"
    exit 1
fi

input_model=$1

# ============== setup variables =====================
if [ ${input_model} = "ising" ]  ||  [ ${input_model} = "epidemic" ] ||  [ ${input_model} = "hard_sphere" ]
then
		echo "***** Compiling: ${input_model} model *****"
else
		echo "***** ERROR: Invalid argument: ${input_model} *****"
		exit
fi

DIR_SRC=./${input_model}
DIR_INCLUDE="./include"
DIR_BIN="./bin"
DIR_DATA="./data"

CC=g++
CFLAGS="-std=c++17 -Wextra -O0 -fbounds-check"
CHECKER=cppcheck
CHECK_FLAG="--enable=all --suppress=missingIncludeSystem"

TARGET=${DIR_SRC}/main.cpp 
OUT_EXECUTABLE=${DIR_BIN}/${input_model}_model_xcutable.out

echo "***** Checking Code Integrity *****"
${CHECKER} ${CHECK_FLAG} ${TARGET}

echo "***** COMPILING THE CODE *****"
echo "***** TARGET: ${TARGET} *****"
echo "***** OUTPUT: ${OUT_EXECUTABLE} *****"
compiling_command="${CC} ${CFLAGS} ${TARGET} -o ${OUT_EXECUTABLE}"

if ${compiling_command}; then
    echo "***** Success!!! *****"
else
    echo "***** Failure!!! *****"
fi


