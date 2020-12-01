#!/bin/bash

echo "#########################################################################"
echo "### Loading needed software versions ####################################" 
echo "#########################################################################"

# Loading CMake version >= 3.8
export PATH=/cvmfs/sft.cern.ch/lcg/releases/CMake/3.17.3-75516/x86_64-centos7-gcc8-opt/bin:${PATH}

# Loading gcc8
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8/x86_64-centos7-gcc8-opt/setup.sh

# Loading ROOT version >= 6.16
source /afs/desy.de/project/ilcsoft/sw/x86_64_gcc82_centos7/root/6.18.04/bin/thisroot.sh