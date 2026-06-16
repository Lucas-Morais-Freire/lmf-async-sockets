#include "UtilsWindows.hpp"

// Windows
#include <winnls.h>
#include <errhandlingapi.h>

// std
#include <iostream>
#include <limits>
#include <optional>

// 1st-party
#include <common.hpp>

namespace Utils {
  std::optional<std::string> utf16ToUtf8(const std::wstring &utf16_str) {
    return utf16ToUtf8(utf16_str.data(), utf16_str.size());
  }

  std::optional<std::string> utf16ToUtf8(const wchar_t *utf16_str, size_t utf16_len) {
    std::optional<std::string> ret;

    if (!utf16_str) {
      std::cerr << "Utils::utf16ToUtf8: Ponteiro nulo passado para a função\n";
      return ret;
    }
    
    if (utf16_len > size_t(std::numeric_limits<int>::max())) {
      std::cerr << "Utils::utf16ToUtf8: Comprimento da string muito grande para a conversão\n";
      return ret;
    }

    if (utf16_len == 0) {
      ret.emplace();
      return ret;
    }

    int utf8_len = WideCharToMultiByte(
      CP_UTF8  , 0,
      utf16_str, sCast<int>(utf16_len),
      nullptr  , 0,
      nullptr  , nullptr
    );

    if (utf8_len == 0) {
      std::cerr << "Utils::utf16ToUtf8: Erro durante conversão. Código: " << GetLastError() << '\n';
      return ret;
    }

    ret.emplace(utf8_len, '\0');

    WideCharToMultiByte(
      CP_UTF8    , 0,
      utf16_str  , sCast<int>(utf16_len),
      ret->data(), utf8_len,
      nullptr    , nullptr
    );

    return ret;
  }



  std::optional<std::wstring> utf8ToUtf16(std::string_view utf8_str) {
    return utf8ToUtf16(utf8_str.data(), utf8_str.size());
  }
  
  
  
  std::optional<std::wstring> utf8ToUtf16(const char *utf8_str, size_t utf8_len) {
    std::optional<std::wstring> ret;

    if (!utf8_str) {
      std::cerr << "Utils::utf8ToUtf16: Ponteiro nulo passado para a função\n";
      return ret;
    }
    
    if (utf8_len > size_t(std::numeric_limits<int>::max())) {
      std::cerr << "Utils::utf8ToUtf16: Comprimento da string muito grande para a conversão\n";
      return ret;
    }

    if (utf8_len == 0) {
      ret.emplace();
      return ret;
    }

    int utf16_len = MultiByteToWideChar(
      CP_UTF8, 0,
      utf8_str, (int)utf8_len,
      nullptr, 0
    );

    if (utf16_len == 0) {
      std::cerr << "Utils::utf16ToUtf8: Erro durante conversão. Código: " << GetLastError() << '\n';
      return ret;
    }
    
    ret.emplace(utf16_len, '\0');

    MultiByteToWideChar(CP_UTF8, 0, utf8_str, (int)utf8_len, ret->data(), utf16_len);
    return ret;
  }
}