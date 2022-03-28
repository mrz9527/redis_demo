cmake_minimum_required(VERSION 3.5)
project(redis)

set(CMAKE_CXX_STANDARD 11)

include_directories(hiredis/include)
link_directories(hiredis/lib)
add_executable(sync_redis sync_redis.cpp)
target_link_libraries(sync_redis hiredis)

