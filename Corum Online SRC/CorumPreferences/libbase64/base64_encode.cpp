#include "base64.hpp"

namespace Base64 {
  std::string _encode_split(std::string in)
  {
    in.resize(3, 0);
    
    char _out[5];
    _out[0] = Base64Table.at((in[0] & 0xFC) >> 2);
    _out[1] = Base64Table.at((in[0] & 0x03) << 4 | (in[1] & 0xF0) >> 4);
    _out[2] = Base64Table.at((in[1] & 0x0F) << 2 | (in[2] & 0xC0) >> 6);
    _out[3] = Base64Table.at((in[2] & 0x3F));
    _out[4] = '\0';
    
    return std::string(_out);
  }
  
  int encode(const std::string* src, std::string* dst)
  {
    for (uint64_t i = 0 ; i < src->length(); i += 3) {
      dst->append( _encode_split(src->substr(i, 3)) );
    }
    
    switch (src->length() % 3) {
    case 0 : 
      break;
    case 1 : 
      dst->erase(dst->length() - 2, 2);
      dst->append("==");
      break; 
    case 2 : 
      dst->erase(dst->length() - 1, 1);
      dst->append("=");
      break;
    }
    
    return 0;
  }
  
  std::string encode(const std::string& src)
  {
    std::string dst;
    for (uint64_t i = 0; i < src.length(); i += 3) {
      dst = dst.append( _encode_split(src.substr(i, 3)) );
    }
    
    switch (src.length() % 3) {
    case 0 : 
      break;
    case 1 : 
      dst.erase(dst.length() - 2, 2);
      dst.append("==");
      break; 
    case 2 : 
      dst.erase(dst.length() - 1, 1);
      dst.append("=");
      break;
    }
    return dst;
  }
};
