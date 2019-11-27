set(BINARY ${CMAKE_PROJECT_NAME})

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources`
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true include/*.h src/*.cpp)

set(SOURCES ${SOURCES})

###############################################################################
## target definitions #########################################################
###############################################################################

add_executable(${BINARY}_run ${SOURCES})

# just for example add some compiler flags
target_compile_options(${BINARY}_run PUBLIC -std=c++1y -Wall -Wfloat-conversion)

# ###############################################################################
# ## dependencies ###############################################################
# ###############################################################################
# 
# # this defines the variables Boost_LIBRARIES that contain all library names
# # that we need to link to
# find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)
# 
# target_link_libraries(example PUBLIC
#   ${Boost_LIBRARIES}
#   gtest_main
#   # here you can add any library dependencies
# )


###############################################################################
## packaging ##################################################################
###############################################################################

add_library(${BINARY}_lib STATIC ${SOURCES})

# install binary in bin folder
install(TARGETS ${BINARY}_run RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
