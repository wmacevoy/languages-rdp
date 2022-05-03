#include "port.h"

std::string operator"" _string(const char8_t* str, std::size_t n) {
  return std::string(reinterpret_cast< const char* >(str),n);  
}

JSON operator"" _JSON(const char8_t* str, std::size_t n) {
  std::string js(reinterpret_cast< const char* >(str),n);
  return JSON::parse(js);
}
