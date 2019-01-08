cmake_minimum_required(VERSION 3.8)

project(zlog-experiment)

add_executable(zlog-experiment main.c)

configure_file(zlog.conf zlog.conf COPYONLY)

target_link_libraries(zlog-experiment zlog)
