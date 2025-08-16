#ifndef __BASE64_HPP__
#define __BASE64_HPP__

#define __DEBUG__ 1

#include <iostream>

#include <array>
#include <string>
namespace Base64
{
  const std::array<char, 64> Base64Table { {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
  } };

  int encode(const std::string*, std::string*);
  int decode(const std::string*, std::string*);
  
  std::string encode(const std::string&);
  std::string decode(const std::string&);
};

#endif
