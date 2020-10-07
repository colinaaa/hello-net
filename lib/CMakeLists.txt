file (GLOB LIB_SOURCES "*.cc" "http/*.cc")
add_library (net STATIC ${LIB_SOURCES})

target_link_libraries (net spdlog::spdlog scn::scn)