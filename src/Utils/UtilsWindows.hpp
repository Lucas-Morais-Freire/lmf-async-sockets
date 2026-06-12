#pragma once

#include <string>
#include <optional>

namespace Utils {
  std::optional<std::string> utf16ToUtf8(const std::wstring &utf16_str);

  std::optional<std::string> utf16ToUtf8(const wchar_t *utf16_str, size_t utf16_len);
};