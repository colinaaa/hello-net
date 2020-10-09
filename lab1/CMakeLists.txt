cmake_minimum_required (VERSION 3.12.0)
project (Netlabs)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/../third_party/spdlog" "./spdlog" EXCLUDE_FROM_ALL)
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/../third_party/scnlib" "./scnlib" EXCLUDE_FROM_ALL)
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/../third_party/docopt" "./docopt" EXCLUDE_FROM_ALL)

include (etc/build_defs.cmake)
include (etc/build_type.cmake)
include (etc/cflags.cmake)

include (etc/doxygen.cmake)

include (etc/clang_format.cmake)
include (etc/clang_tidy.cmake)

include_directories ("${PROJECT_SOURCE_DIR}/lib")

add_subdirectory ("${PROJECT_SOURCE_DIR}/lib")

add_subdirectory ("${PROJECT_SOURCE_DIR}/apps")
