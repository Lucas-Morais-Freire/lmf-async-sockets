#pragma once

#define scast static_cast
#define dcast dynamic_cast
#define rcast reinterpret_cast
#define ccast const_cast



#define COLETOR_DELETE_MOVE_COPY(Class)   \
Class(const Class &) = delete;            \
Class(Class &&) = delete;                 \
Class &operator=(const Class &) = delete; \
Class &operator=(Class &&) = delete;



#define COLETOR_DELETE_MOVE(Class)   \
Class(Class &&) = delete;            \
Class &operator=(Class &&) = delete;



#define COLETOR_DELETE_COPY(Class)        \
Class(const Class &) = delete;            \
Class &operator=(const Class &) = delete;



#define COLETOR_DEFAULT_MOVE_COPY(Class)   \
Class(const Class &) = default;            \
Class(Class &&) = default;                 \
Class &operator=(const Class &) = default; \
Class &operator=(Class &&) = default;


#define main(argc_decl, argv_decl) \
main_post_setup(argc_decl, argv_decl); \
void os_setup(int &argc, const char **const &argv); \
int main(argc_decl, argv_decl) { \
  os_setup(argc, argv); \
  return main_post_setup(argc, argv); \
} \
int main_post_setup(argc_decl, argv_decl)
