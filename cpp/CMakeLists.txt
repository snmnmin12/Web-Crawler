project(Crawler)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++11)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(sources src/main.cpp)
set(HW1.4  HW1.4/main.cpp HW1.4/network.cpp HW1.4/sharedData.cpp HW1.4/Statis.cpp HW1.4/util.cpp HW1.4/urlExtraction.cpp)


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

include_directories(/usr/local/include)
include_directories(/usr/local/opt/openssl/include)
link_directories(/usr/local/lib)
link_directories(/usr/local/opt/openssl/lib)
link_directories(/usr/local/Cellar/libuv/1.11.0/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

#add_executable(server src/server.cpp)
#target_link_libraries(server z ssl uv uWS)


add_executable(crawler ${HW1.4})
target_link_libraries(crawler z ssl uv uWS)