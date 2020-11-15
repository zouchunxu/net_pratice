add_executable(pingclient pingclient.cpp)
target_link_libraries(pingclient yolanda)

add_executable(pingserver pingserver.cpp)
target_link_libraries(pingserver yolanda)