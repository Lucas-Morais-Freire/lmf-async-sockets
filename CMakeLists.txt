cmake_minimum_required(VERSION 3.31)
project(MAP-Collector)

include("Utils.cmake")

# Usamos o padrão do C++20.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
unset(CMAKE_CXX_VERBOSE_FLAG)
unset(CMAKE_C_VERBOSE_FLAG)

### Configuração de flags

set(CMAKE_CXX_FLAGS_RELEASE "-Werror -O3 -DNDEBUG -march=x86-64 -mtune=generic")
# Caso esteja compilando no modo 'Debug', as libs do compilador serão linkadas dinamicamente. Neste caso, cuidado
# com o caminho das libs do compilador. Se elas estiverem em um caminho que o executável não tenha permissão para abrir,
# o serviço não será executado (por exemplo, no Windows, o serviço roda na conta LocalSystem que não tem permissão para
# acessar alguns diretórios de usuário).
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG -DCOLETOR_DEBUG")

# Restringir ao compilador GCC por hora.
if(NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  message(FATAL_ERROR "Este projeto aceita apenas o GNU C Compiler como compilador.")
endif()

set_os(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0A00 -DCOLETOR_WINDOWS" # Windows
  "${CMAKE_CXX_FLAGS} -DCOLETOR_LINUX"                         # Linux
)

set_os(
  CMAKE_CXX_FLAGS_RELEASE
  "${CMAKE_CXX_FLAGS_RELEASE} -static"
  "${CMAKE_CXX_FLAGS_RELEASE} -static-libgcc -static-libstdc++"
)

message(STATUS_IMPORTANT "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message(STATUS_IMPORTANT "CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS_IMPORTANT "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")

# Configurações baseadas no tipo de build
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "O projeto está sendo configurado para 'Release'.")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "O projeto está sendo configurado para 'Debug'.")
else()
    message(FATAL_ERROR "O CMAKE_BUILD_TYPE escolhido não é suportado.")
endif()

add_subdirectory(src)