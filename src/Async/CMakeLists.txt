add_library(Async STATIC
  AwaiterToken.cpp
  Escalonador.cpp
  Promise.cpp
  Trava.cpp
  Trava/Awaiter.cpp
)

target_include_directories(Async
  PUBLIC ${COLETOR_INCLUDE}
)