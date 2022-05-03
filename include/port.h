#pragma once

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>
#include <tgmath.h>
#include <stdexcept>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <iostream>
#include <string>
#include <version>
#include <memory>
#include <functional>

#include "json.hpp"

using JSON = nlohmann::json;

#if !defined(__cpp_char8_t)
using char8_t = char;
namespace std {
  using u8string = string;
}
#endif

std::string operator"" _string(const char8_t* str, std::size_t n);
JSON operator"" _JSON(const char8_t* str, std::size_t n);
