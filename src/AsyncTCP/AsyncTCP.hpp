#pragma once

#if defined(COLETOR_WINDOWS)
#include "AsyncTCPWindows.hpp"
#else
#error "Este projeto não pode ser compilado para este sistema"
#endif