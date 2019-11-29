echo "#########################################################################"
echo "### Loading needed software versions ####################################" 
echo "#########################################################################"

# Loading CMake version >= 3.8
export PATH=/cvmfs/sft.cern.ch/lcg/releases/CMake/3.8.2-ece19/x86_64-slc6-gcc8-opt/bin:${PATH}

# Loading gcc8
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8/x86_64-slc6-gcc8-opt/setup.sh

# Loading ROOT version >= 6.16
source /afs/desy.de/project/ilcsoft/sw/x86_64_gcc49_sl6/root/build-6.16.00/bin/thisroot.sh