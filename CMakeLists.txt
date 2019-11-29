cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(PrEW) # Project name

# External libraries
link_directories(spdlog/build)

# Common packages
find_package( ROOT 6.16 REQUIRED ) 
include_directories( SYSTEM ${ROOT_INCLUDE_DIRS} )
link_libraries( ${ROOT_LIBRARIES} )
add_definitions( ${ROOT_DEFINITIONS} )

# Header files that can be included using #include
include_directories(source/prew/include)
include_directories(spdlog/include)

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