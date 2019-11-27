# PrEW - Precision Electro-Weak fits


## Installation

1. Clone and build spdlog from github:
  ```sh
    git clone https://github.com/gabime/spdlog.git spdlog
    cd spdlog && mkdir build && cd build
    cmake .. && make -j
  ```
2. Clone the googletest repository from github:
  ```sh
    git clone https://github.com/google/googletest.git googletest
  ```
3. Compile the code:
  ```sh
    cd macros
    chmod u+x compile
    ./compile
  ```