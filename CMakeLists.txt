cmake_minimum_required(VERSION 3.0.0)
project(twitter_listener VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("/home/ville/projects/json/include" "home/ville/projects/curl/include")

link_directories("/usr/local/lib")

link_libraries("curl")

add_library(twitter_listener twitter_listener.hpp twitter_listener.cpp)

add_executable(twitter_listener_cli twitter_listener_cli.cpp twitter_listener.hpp twitter_listener.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
