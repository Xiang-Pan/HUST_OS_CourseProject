#cmake verson
cmake_minimum_required(VERSION 3.2)

#project name
PROJECT(myfs)

#head file path
INCLUDE_DIRECTORIES(include)

#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

SET(src_total ${DIR_SRCS} src/superblock.cpp)

#add executable file，
ADD_EXECUTABLE(${PROJECT_NAME} ${src_total})

find_package(Boost REQUIRED COMPONENTS serialization)

if(NOT Boost_FOUND)
    message("Not found Boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
message("${Boost_INCLUDE_DIRS}")
message("${Boost_LIBRARIES}")

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})