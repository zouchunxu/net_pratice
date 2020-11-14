if (EPOLL_EXISTS)
    set(yolanda_lib
            log.cpp
            sock_ntop.cpp
            read.c
            tcp_server.cpp
            tcp_client.cpp
            event_loop.cpp
            channel.cpp
            acceptor.cpp
            channel_map.cpp
            poll_dispatcher.cpp
            thread_pool.cpp
            event_loop_thread.cpp
            utils.cpp
            epoll_dispatcher.cpp
            buffer.cpp
            tcp_connection.cpp
            http_server.cpp
            http_request.cpp
            http_response.cpp
            )
else ()
    set(yolanda_lib
            log.cpp
            sock_ntop.cpp
            read.c
            tcp_server.cpp
            tcp_client.cpp
            event_loop.cpp
            channel.cpp
            acceptor.cpp
            channel_map.cpp
            poll_dispatcher.cpp
            thread_pool.cpp
            event_loop_thread.cpp
            utils.cpp
            buffer.cpp
            tcp_connection.cpp
            http_server.cpp
            http_request.cpp
            http_response.cpp
            )
endif ()


add_library(yolanda ${yolanda_lib})
target_link_libraries(yolanda pthread)

install(TARGETS yolanda DESTINATION lib)

#file(GLOB HEADERS "*.h")
#install(FILES ${HEADERS} DESTINATION include/)

