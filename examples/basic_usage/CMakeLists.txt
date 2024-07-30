project(basic_usage)

add_executable(simplestring simplestring.c)
target_link_libraries(simplestring scs)

add_executable(hugestring hugestring.c)
target_link_libraries(hugestring scs)