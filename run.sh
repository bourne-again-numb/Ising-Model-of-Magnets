#!/bin/bash

input_model=$1

# ============== setup variables =====================
if [ ${input_model} = "ising_model" ]				# for ising model
then
		model_type=${input_model}
		initial_spin=1
elif [ ${input_model} = "epidemic_model" ]			# for the epidemic model
then
		model_type=${input_model}
		density=1
else
		echo *ERROR*: Invalid argument: $1
		exit
fi

independent_runs=1
lx=50
ly=50
lz=50
tstar_i=1.000
tstar_f=3.000
tstar_d=1.0

mcsteps=10000
eqsteps=1000
sample_freq=100

exchange_energy=1.000
lattice_type="2Dscc"
# =====================================================

#now=`date +"%Y-%m-%d_%T"`
#now=`date +"%d-%b-%Y-%H%M%S"`
main_dir=`pwd`

input_file=${main_dir}/data/${model_type}_input.csv
executable_file=${main_dir}/bin/${model_type}_xcutable.out

# make the code
./make.sh ${input_file} | tee >(ts "%d-%m-%y %H_%M_%S" > ${log_file_path})

for tstar in $(seq $tstar_i $tstar_d $tstar_f) # loop over temperature values
do
		cd ${main_dir}
		work_dir_name=${model_type}_${lattice_type}_tstar${tstar}_J${exchange_energy}_${lx}x${ly}x${lz}_${mcsteps}_${eqsteps}_${sample_freq}
		work_dir_path=${main_dir}/data/${work_dir_name}
		mkdir -p ${work_dir_path}
		echo $work_dir_path

		# create the input file
		echo lattice_type,${lattice_type} > ${input_file}
		echo lx,${lx} >> ${input_file}
		echo ly,${ly} >> ${input_file}
		echo lz,${lz} >> ${input_file}
		echo tstar,${tstar} >> ${input_file}
		echo mcsteps,${mcsteps} >> ${input_file}
		echo eqsteps,${eqsteps} >> ${input_file}
		echo sample_freq,${sample_freq} >> ${input_file}
		echo initial_spin,${initial_spin} >> ${input_file}
		echo exchange_energy,${exchange_energy} >> ${input_file}
		
		
		for run in $(seq 1 1 $independent_runs) # loop over independent runs
		do
				run_dir=${main_dir}/data/${work_dir_name}/run_${run}
				log_file_path=${main_dir}/logs/${work_dir_name}_${run}.log
				mkdir -p ${run_dir}
				cd ${run_dir}
				cp ${input_file} ${run_dir}
				cp ${executable_file} ${run_dir}

				# run the command
				${executable_file} |  tee >(ts "%d-%m-%y %H_%M_%S" > ${log_file_path})
			 
				# echo $log_file_path

		done												# end loop over independent runs
		
done														# end loop over temperature values

# clean up executable
cd ${main_dir}
./clean.sh ${input_model}

