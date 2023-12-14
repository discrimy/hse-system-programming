cmake_minimum_required(VERSION 3.10)
project(md5_crack)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS thread REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(md5_crack main.cpp ctpl.h)

target_link_libraries(md5_crack PRIVATE Boost::thread OpenSSL::SSL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")