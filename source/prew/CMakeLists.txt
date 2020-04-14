set(BINARY ${CMAKE_PROJECT_NAME})

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources`
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true include/*/*.h src/*/*.cpp src/main.cpp)

set(SOURCES ${SOURCES})

###############################################################################
## target definitions #########################################################
###############################################################################

add_library(${BINARY} ${SOURCES})

# just for example add some compiler flags
target_compile_options(
  ${BINARY} PUBLIC -std=c++1y 
  # Compiler warning/error flags
  -Wall -Wfloat-conversion -Wextra -Wunreachable-code -Wuninitialized 
  -pedantic-errors -Wold-style-cast -Wno-error=unused-variable
  -Wfloat-equal
  # Compiler optimisation
  -O3
)

###############################################################################
## dependencies ###############################################################
###############################################################################

# Link the needed libraries so that its compiled code can be used
target_link_libraries(
  ${BINARY} PUBLIC 
  ${SPDLOG_LIB} # Logging
  ROOT::Minuit2 # Minimization
)

###############################################################################
## packaging ##################################################################
###############################################################################

# install binary in bin folder
install(
  TARGETS ${BINARY} 
  ARCHIVE DESTINATION lib
  COMPONENT library
)
