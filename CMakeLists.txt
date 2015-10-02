project(WordLinks)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} -lboost_filesystem -lboost_system)

# For C++11 functionnalities
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
