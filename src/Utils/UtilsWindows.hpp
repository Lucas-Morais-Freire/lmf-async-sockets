#pragma once

#include <string>
#include <optional>

namespace Utils {
  std::optional<std::string> utf16ToUtf8(std::wstring_view utf16_str);

  std::optional<std::string> utf16ToUtf8(const wchar_t *utf16_str, size_t utf16_len);

  std::optional<std::wstring> utf8ToUtf16(std::string_view utf8_str);

  std::optional<std::wstring> utf8ToUtf16(const char *utf8_str, size_t len);
};