#pragma once

#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

namespace lzw {

class lzw_encoder {
 public:
  lzw_encoder(std::istream &is, std::ostream &os);

  void encode();

 private:
  uint32_t current_code = 0;
  std::string current;

  std::unordered_map<std::string, uint32_t> codebook;
  std::istream &is;
  std::ostream &os;
};

class lzw_decoder {
 public:
  lzw_decoder(std::istream &is, std::ostream &os);

  void decode();

 private:
  std::vector<std::string> codebook;
  std::optional<uint32_t> prev;
  std::istream &is;
  std::ostream &os;
};
}  // namespace lzw