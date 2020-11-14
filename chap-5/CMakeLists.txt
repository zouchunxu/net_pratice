
add_executable(chap5_server server.cpp)
target_link_libraries(chap5_server yolanda)

add_executable(chap5_client client.cpp)
target_link_libraries(chap5_client yolanda)