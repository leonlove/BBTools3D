cmake_minimum_required(VERSION 3.20)

# 工程名称
project(BBTools3D)

# 设置C++编译选项
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Grouped compiler settings
IF ((CMAKE_C_COMPILER_ID MATCHES "GNU") AND NOT CMAKE_COMPILER_IS_MINGW)
  # hide all not-exported symbols
  # SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb -std=c++11 -fPIC") 
  # SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -fPIC -std=c++11")
ELSEIF(MSVC)
  # enable multi-core compilation with MSVC
  add_compile_options(/MP)
  add_compile_options(/MD)
  add_compile_options(/bigobj)
  add_compile_options(/utf-8)
ENDIF()

# 往CMAKE_MODULE_PATH变量中添加${CMAKE_SOURCE_DIR}/cmake路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# 设置第三方库路径
set(THIRD_PARTY_INSTALL_DIR ${CMAKE_SOURCE_DIR}/third/install)

# 添加Assimp
# find_package(assimp REQUIRED)
include_directories(${THIRD_PARTY_INSTALL_DIR}/assimp/include)
link_directories(${THIRD_PARTY_INSTALL_DIR}/assimp/lib)

# 添加laslib
# find_package(laslib REQUIRED)
include_directories(${THIRD_PARTY_INSTALL_DIR}/laslib/include)
link_directories(${THIRD_PARTY_INSTALL_DIR}/laslib/lib)

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


# 链接第三方库
target_link_libraries(BBTools3D
    ${THIRD_PARTY_INSTALL_DIR}/assimp/lib/assimp-vc140-mt.lib
	${THIRD_PARTY_INSTALL_DIR}/laslib/lib/Laslib.lib
)

