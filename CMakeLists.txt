cmake_minimum_required(VERSION 3.16)

project(webserver LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(UNIX AND NOT APPLE)
    add_definitions(-DUNIX_SYSTEM)
elseif(APPLE)
    add_definitions(-DAPPLE_SYSTEM)
else()
    add_definitions(-DWINDOWS_SYSTEM)
endif()

find_package(LIBEVENT REQUIRED
    PATHS /Users/wenpengwu/project/c_pp/libevent-2.1.12-stable/build/Qt_6_6_3_for_macOS-Debug
NO_DEFAULT_PATH)

if(LIBEVENT_FOUND)
    message("found LIBEVENT_FOUND")
    include_directories(${LIBEVENT_INCLUDE_DIRS})
endif()


add_executable(webserver
    parse.h
    parse.cpp
    utility.h utility.cpp
    tinyhttp.h tinyhttp.cpp)


add_executable(kqueue_demo

    kqueue_demo.cpp)




add_executable(pratic pratic.cpp)


add_executable(accpet_client accept_client.cpp)

add_executable(udp_client udp_client.cpp)



add_executable(accept_server accept_server.cpp)
add_executable(select_server select_server.cpp)
add_executable(poll_server poll_server.cpp)

add_executable(libevent_server libevent_server.cpp)
target_link_libraries(libevent_server
  PUBLIC
  ${LIBEVENT_LIBRARIES}
)


add_executable(udp_server udp_server.cpp)


include(GNUInstallDirs)
install(TARGETS kqueue_demo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
