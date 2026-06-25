#pragma once

#define sCast static_cast
#define dCast dynamic_cast
#define rCast reinterpret_cast
#define cCast const_cast



#define COLETOR_DELETE_MOVE_COPY(Class)   \
Class(const Class &) = delete;            \
Class(Class &&) = delete;                 \
Class &operator=(const Class &) = delete; \
Class &operator=(Class &&) = delete;
