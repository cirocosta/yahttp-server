#     --    config

cmake_minimum_required(VERSION 2.8)

# Turn it ON: -Dtest=ON
option(test "Include Tests." OFF)

set(PROJECT_NAME yahttp-server)
project(${PROJECT_NAME} C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
    message(STATUS "Please use a different C++ compiler.")
endif()


#     --    source

include_directories(${PROJECT_SOURCE_DIR}/includes)
add_subdirectory(src/)


#     --    testing

if (test)
  enable_testing()
  add_subdirectory(tests/)
endif(test)


