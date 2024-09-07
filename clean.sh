#!/bin/bash -f

input_model=$1

# ============== setup variables =====================
if [ ${input_model} = "ising_model" ]  ||  [ ${input_model} = "epidemic_model" ]				# for ising model
then
		echo Cleaning: ${input_model} Data
else
		echo *ERROR*: Invalid argument: $1
		exit
fi

DIR_SRC="./src"
DIR_INCLUDE="./include"
DIR_BIN="./bin"
DIR_DATA="./data"

INPUT_FILE=${DIR_DATA}/${input_model}_input.csv
OUT_EXECUTABLE=${DIR_BIN}/${input_model}_xcutable.out
RESULTS_FILES=${DIR_DATA}/${input_model}_results.csv ${DIR_DATA}/random_numbers.csv ${DIR_DATA}/${input_model}_vmd.xyz

rm -rfv ${OUT_EXECUTABLE} ${RESULTS_FILES} ${INPUT_FILE}

