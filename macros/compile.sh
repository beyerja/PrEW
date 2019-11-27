#!/bin/bash

# -------------------------------------------------#
# Bash script for compiling the processor of the
# analysis.
# To be run simply by using: ./run_make.sh [-rebuild]
# -------------------------------------------------#

if [[  $# > 1 ]]  ; then
	echo "usage: ./compile.sh [-rebuild]"
	exit
fi

# -------------------------------------------------#
# Optional input parameter:
# Should everything be rebuild? 
# (-> will delete content of build folder)
if [ $# == 1 ]; then
  REBUILD=true
fi

# -------------------------------------------------#
# Check if necessary directories are in place

dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
home_folder=${dir}/..

if [ -d ${home_folder}/build  ] ; then
	echo "Already have  --build-- filter to compile"
  # If requested remove build folder content
  if [ ${REBUILD} ]; then 
    echo "Rebuild requested => Recreating build folder"
    rm -r ${home_folder}/build
    mkdir ${home_folder}/build
  fi
else
	echo "no build filter"
	mkdir ${home_folder}/build
fi

# -------------------------------------------------#
# Move to build directory and to build framework
# (cmake + make)

cd ${home_folder}/build

if [ -f "make.output"  ] ; then
	rm make.output
fi

echo "begin to config"
echo
cmake .. 
wait 
echo
echo "begin to make" 
make 
wait
echo
echo "begin to make install" 
make install # >> make.output  2>&1
wait
echo 
echo "begin to test"
./bin/PrEW_tst
# echo "begin to make install"
# echo
# make install # >> make.output 2>&1
# echo 
# WARNINGMESSAGE=$(grep "warning" -irn ./make.output)
# echo "check for warning message"
# echo ${WARNINGMESSAGE}
# echo 
# ERRORMESSAGE=$(grep "error" -irn ./make.output)
# echo "check for error message"
# echo ${ERRORMESSAGE}
# echo
echo "end"
