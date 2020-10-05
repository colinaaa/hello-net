find_library (LIBPCAP pcap)
find_library (LIBPTHREAD pthread)
macro (add_exec exec_name)
    add_executable ("${exec_name}" "${exec_name}.cc")
    target_link_libraries ("${exec_name}" ${ARGN} net ${LIBPTHREAD})
endmacro (add_exec)
