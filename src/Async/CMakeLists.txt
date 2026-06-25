add_library(Async STATIC
  AwaiterToken.cpp Escalonador.cpp
)

target_include_directories(Async
  PUBLIC ${COLETOR_INCLUDE}
)