set_os(SRCS_OS
  "AsyncTCPWindows.cpp;DNSLookupWindows.cpp"
  ""
)

add_library(AsyncTCP STATIC
  ${SRCS_OS}
)

set_os(LIBS_OS
  "ws2_32"
  ""
)

target_link_libraries(AsyncTCP
  PRIVATE Utils Async ${LIBS_OS}
)

target_include_directories(AsyncTCP
  PUBLIC ${COLETOR_INCLUDE}
)