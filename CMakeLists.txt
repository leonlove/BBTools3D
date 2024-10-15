cmake_minimum_required(VERSION 3.20)

# 工程名称
project(BBTools3D)

# 设置C++编译选项
set(CMAKE_CXX_STANDARD 11)

# 查找指定目录下的所有源文件，并将名称保存到制定变量名
# 但是不会把头文件添加进去。需使用另一种办法 使用FILE变量
# aux_source_directory(./src DIR_SRCS)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE header_files "${SRC_DIR}/*.h" "${SRC_DIR}/*.inl")
file(GLOB_RECURSE source_files "${SRC_DIR}/*.cpp")

# TREE "${CMAKE_CURRENT_SOURCE_DIR}" 告诉CMake根据文件相对于 ${CMAKE_CURRENT_SOURCE_DIR} 的路径来组织它们。
# PREFIX "Header Files" 为所有的源代码组添加了一个前缀 "Header Files"
# FILES ${header_files} 指定了要组织的所有头文件
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Header Files" FILES ${header_files})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${source_files})

# 编译源码生成目标
add_executable(BBTools3D ${source_files} ${header_files})

