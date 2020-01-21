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
    git clone https://github.com/gabime/spdlog.git external/spdlog
    cd external/spdlog && mkdir build && cd build
    cmake .. && make -j
    cd ../../..
  ```
4. Clone the googletest repository from github:
  ```sh
    git clone https://github.com/google/googletest.git external/googletest
  ```
5. Unzip data files needed for testing:
  ```sh
    cd testdata && unzip *.zip && cd ..
  ```
6. Compile the code:
  ```sh
    cd macros && chmod u+x compile.sh && ./compile.sh && cd ..
  ```