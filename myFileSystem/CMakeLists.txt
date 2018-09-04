#1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.2)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(myfs)

#3.head file path，头文件目录
INCLUDE_DIRECTORIES(
        include
)

#4.source directory，源文件目录
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

SET(src_total
        ${DIR_SRCS}
        include/macro.h)

#6.add executable file，添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${src_total})

find_package(Boost REQUIRED COMPONENTS
        # regex
        serialization   # 我的工程中只使用了 boost 的 filesystem 功能,因此这里只有一个组件
        )
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
message("${Boost_INCLUDE_DIRS}")
message("${Boost_LIBRARIES}")

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})