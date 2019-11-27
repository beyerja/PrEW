cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

project(PrEW) # Project name


# Header files that can be included using #include
include_directories(source/prew/include)
include_directories(spdlog/include)

# External libraries
link_directories(spdlog/build)

# Code subdirectories with own CMakelists
add_subdirectory(source/prew)
add_subdirectory(source/tests)
add_subdirectory(googletest)

# Install in local folder instead of system
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/bin)

# Package name and version
set(CPACK_PACKAGE_NAME "PrEW")
set(CPACK_PACKAGE_VERSION "1.0.0")

# we don't want to split our program up into several things
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)