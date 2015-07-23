#     --    yahttp-parser

include(ExternalProject)
ExternalProject_Add(
  yahttp-parser
  GIT_REPOSITORY https://github.com/cirocosta/yahttp-parser.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/yahttp-parser
  INSTALL_COMMAND ""
)

add_library(libyahttp-parser IMPORTED STATIC GLOBAL)
add_dependencies(libyahttp-parser yahttp-parser)

ExternalProject_Get_Property(yahttp-parser source_dir binary_dir)
set_target_properties(libyahttp-parser PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libyahttp-parser.a"
)
include_directories("${source_dir}/includes")

