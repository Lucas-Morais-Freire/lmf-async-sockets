set_os(
  SRCS_OS
  "UtilsWindows.cpp"
  ""
)

add_library(Utils STATIC
  ${SRCS_OS}
)

target_include_directories(Utils
  PUBLIC ${COLETOR_INCLUDE}
)