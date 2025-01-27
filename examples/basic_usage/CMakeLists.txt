project(basic_usage)

add_executable(simplestring simplestring.c)
target_link_libraries(simplestring scs)

add_executable(hugestring hugestring.c)
target_link_libraries(hugestring scs)

add_executable(httpserver httpserver.c)
target_link_libraries(httpserver scs)

add_executable(protectviolation protectviolation.c)
target_link_libraries(protectviolation scs)