cmake_minimum_required(VERSION 3.0.0)
project(twitter_listener VERSION 0.1.0)

include(CTest)
enable_testing()

find_package( CURL REQUIRED )

add_library(twitter_listener twitter_listener.hpp twitter_listener.cpp)

add_executable(twitter_listener_cli twitter_listener_cli.cpp twitter_listener.hpp twitter_listener.cpp)

target_link_libraries(twitter_listener PRIVATE  CURL::libcurl)

target_link_libraries(twitter_listener_cli PRIVATE  CURL::libcurl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
