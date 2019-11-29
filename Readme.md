# PrEW - Precision Electro-Weak fits


## Installation

1. Make sure your software is up-to-date (ROOT, gcc, cmake, ...) *or* load using macro (only on NAF):
  ```sh
    cd macro && source load_env.sh && cd ..
  ```
2. Clone and build spdlog from github:
  ```sh
    git clone https://github.com/gabime/spdlog.git spdlog
    cd spdlog && mkdir build && cd build
    cmake .. && make -j
    cd ../..
  ```
3. Clone the googletest repository from github:
  ```sh
    git clone https://github.com/google/googletest.git googletest
  ```
4. Compile the code:
  ```sh
    cd macros && chmod u+x compile.sh && ./compile.sh && cd ..
  ```