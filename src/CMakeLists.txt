

set_os(
  TRANSLATION_UNITS
  "main.cpp;commonWindows.cpp"
  "main.cpp"
)

add_executable(MAP-Collector
  ${TRANSLATION_UNITS}
)

set(COLETOR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(Utils)
add_subdirectory(Async)
add_subdirectory(AsyncTCP)

target_link_libraries(MAP-Collector
  PRIVATE Async
)

target_include_directories(MAP-Collector
  PUBLIC ${COLETOR_INCLUDE}
)

install(TARGETS MAP-Collector DESTINATION "${CMAKE_SOURCE_DIR}/bin")