#include <windows.h>

void os_setup(int &argc, const char **const &argv) {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
}