set(BINARY ${CMAKE_PROJECT_NAME}_tst)

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources`
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

###############################################################################
## target definitions #########################################################
###############################################################################

add_executable(${BINARY} ${TEST_SOURCES})

# ###############################################################################
# ## testing ####################################################################
# ###############################################################################

add_test(NAME ${BINARY} COMMAND ${BINARY})

###############################################################################
## packaging ##################################################################
###############################################################################

# just for example add some compiler flags
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

install(TARGETS ${BINARY} RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)