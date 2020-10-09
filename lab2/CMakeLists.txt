cmake_minimum_required(VERSION 3.10)
project(StopWait)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_C_COMPTLER GCC)
set (CMAKE_CXX_STANDARD 17)

include_directories (${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

find_library (NETSIM_LIB libnetsim.a ${PROJECT_SOURCE_DIR}/lib)

add_executable (gbn src/GoBackN.cc src/GBNRdtReceiver.cc src/GBNRdtSender.cc)
target_link_libraries (gbn ${NETSIM_LIB})

file (GLOB StopWaitSRC ${PROJECT_SOURCE_DIR}/src/StopWait*.cc)

add_executable (stop_wait ${StopWaitSRC})
target_link_libraries (stop_wait ${NETSIM_LIB})
