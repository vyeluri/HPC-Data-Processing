#!/bin/bash
#PBS -N L
#PBS -q fast
#PBS -l nodes=2:ppn=4
#PBS -l walltime=02:30:00

echo $PBS_O_HOST

cd $PBS_O_WORKDIR 

split -b 1024m /home/vyeluri5/final/CCCdata-large.txt   -- Cmd to split large file in to small files

mpirun -np 2 word_count_Large /home/vyeluri5/final/xaa /home/vyeluri5/final/xab /home/vyeluri5/final/xac /home/vyeluri5/final/xad /home/vyeluri5/final/xae effective

-- Given the path of a small files as an arguments to the .cpp main function 
