#     --    Configure GTEST

find_package(Threads REQUIRED)
include(ExternalProject)
ExternalProject_Add(
  gtest
  GIT_REPOSITORY https://github.com/cirocosta/googletest.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  INSTALL_COMMAND ""
)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
include_directories("${source_dir}/include")


#     --    TEST
add_executable(dummy_test main_test.cc dummy_test.cc)
target_link_libraries(dummy_test libgtest
  server
)

add_test(NAME dummy_test COMMAND dummy_test)

add_custom_command(
  TARGET dummy_test PRE_BUILD
  COMMAND ${CMAKE_COMMAND}
  -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:dummy_test>/assets
)


