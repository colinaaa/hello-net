file (GLOB LIB_SOURCES "*.cc")
add_library (net STATIC ${LIB_SOURCES})

target_link_libraries (net fmt::fmt spdlog::spdlog)