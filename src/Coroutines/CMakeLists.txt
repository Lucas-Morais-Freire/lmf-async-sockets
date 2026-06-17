add_library(Coroutines STATIC
  AwaiterToken.cpp
)

target_include_directories(Coroutines
  PUBLIC ${COLETOR_INCLUDE}
)