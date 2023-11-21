cmake_minimum_required(VERSION 3.27)

project(p2p_messaging)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options asio date_time)
set(BOOST_ENABLE_CMAKE ON)
include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_PROGRESS TRUE
  GIT_TAG boost-1.82.0
)
FetchContent_MakeAvailable(Boost)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE 
  Boost::asio
  Boost::filesystem
  Boost::thread
  Boost::program_options
)