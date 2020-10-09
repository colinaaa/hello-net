cmake_minimum_required(VERSION 3.10)
project(StopWait)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_C_COMPTLER GCC)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/../third_party/spdlog" "./spdlog" EXCLUDE_FROM_ALL)
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/../third_party/scnlib" "./scnlib" EXCLUDE_FROM_ALL)
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/../third_party/docopt" "./docopt" EXCLUDE_FROM_ALL)

include_directories (${PROJECT_SOURCE_DIR}/include)

file (GLOB StopWaitSRC ${PROJECT_SOURCE_DIR}/src/StopWait*.cc)
file (GLOB GoBackNSRC ${PROJECT_SOURCE_DIR}/src/GBN*.cc)
file (GLOB SelectiveRepeatSRC ${PROJECT_SOURCE_DIR}/src/SR*.cc)

find_library (NETSIM_LIB libnetsim.a ${PROJECT_SOURCE_DIR}/lib)

add_executable (stop_wait ${StopWaitSRC})
add_executable (gbn ${GoBackNSRC} ${PROJECT_SOURCE_DIR}/src/GoBackN.cc)
add_executable (sr ${SelectiveRepeatSRC} ${PROJECT_SOURCE_DIR}/src/SelectiveRepeat.cc)

target_link_libraries (sr ${NETSIM_LIB} spdlog::spdlog)
target_link_libraries (stop_wait ${NETSIM_LIB} spdlog::spdlog)
target_link_libraries (gbn ${NETSIM_LIB} spdlog::spdlog)
