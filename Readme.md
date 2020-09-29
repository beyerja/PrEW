# PrEW - Precision Electro-Weak fits

## Installation

1. Make sure your software is up-to-date (ROOT, gcc, cmake, ...) *or* load using macro (only on NAF):
 ```sh
 cd macros && source load_env.sh && cd ..
 ```
2. Create subdirectory for external packages:
 ```sh
 mkdir external
 ```
3. Clone and build spdlog from github:
 ```sh
 git clone -b 'v1.7.0' --depth 1 https://github.com/gabime/spdlog.git external/spdlog
 cd external/spdlog && mkdir build && cd build
 cmake .. && make -j
 cd ../../..
 ```
4. Clone the googletest repository from github:
 ```sh
 git clone -b 'release-1.10.0' --depth 1 https://github.com/google/googletest.git external/googletest
 ```
5. Unzip data files needed for testing:
 ```sh
 cd testdata && unzip *.zip && cd ..
 ```
6. Compile the code:
 ```sh
 cd macros && chmod u+x compile.sh && ./compile.sh && cd ..
 ```
 The compilation can also be done in multithreaded mode using `./compile.sh --jobs=N_jobs`.
 
 
### Idea

`PrEW` was developed on the idea to decouple the core computing-heavy chi-squared minimization from the complex and complicated formulation of bin predictions for observable distributions at e+e- colliders.

A core fitting framework provides a fast and general way to fit a parameter-dependent prediction to a large number of bins. 
This core has no knowledge of the physics and must be provided with the measured bin values, parameters, and the bin prediction functions. 
The latter describe how the prediction of the bin depends on the parameters.
With those components available, a chi-squared minimization is performed using the `ROOT::Minuit2` framework.

The rest of the `PrEW` framework is dedicated to setting up the components for the core fit from human-readable physics instructions provided by the user.

### Usage

The current usage of `PrEW` framework itself is still quite complicated.
It is recommended to use classes provided in the `PrEWUtils` package.
An example for such a usage is provided in the `PrEWRunRK` package.
 
## PrOut

`PrEW` contains a `Python` module called `PrOut` that provides classes for the interpretation of fit output files.


### Usage

The module can be loaded into a python script using
```Py
import sys
sys.path.append("INSERT_YOUR_PrEW_PATH/source/prout")
import PrOut
```
(wherein the correct path to your PrEW installation need to be set).

The module classes can be used to interpret an output file and read them into python class objects.

```Py
reader = PrOut.Reader("your_PrEW_output_file.out")
reader.read()
results = reader.run_results
```
For details on the result class objects please see the `PrOut` source code.
 
 