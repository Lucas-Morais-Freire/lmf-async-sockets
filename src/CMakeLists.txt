add_executable(MAP-Collector
  main.cpp
)

set(COLETOR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(Utils)
add_subdirectory(Coroutines)
add_subdirectory(AsyncTCP)

target_link_libraries(MAP-Collector
  PRIVATE AsyncTCP
)

target_include_directories(MAP-Collector
  PUBLIC ${COLETOR_INCLUDE}
)

install(TARGETS MAP-Collector DESTINATION "${CMAKE_SOURCE_DIR}/bin")