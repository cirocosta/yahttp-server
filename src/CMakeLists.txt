add_library(server Server.cc)
add_executable(yahttp-server main.cc)

target_link_libraries(yahttp-server
  server
)

