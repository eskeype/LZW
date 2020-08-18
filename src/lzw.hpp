#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

namespace lzw {

struct Trie {
  uint32_t code;
  std::array<std::unique_ptr<Trie>, 256> children;

  Trie() = default;
  Trie(uint32_t code_) : code(code_) {}
};

class lzw_encoder {
 public:
  lzw_encoder(std::istream &is, std::ostream &os);

  void encode();

 private:
  uint32_t current_code = 0;
  std::unique_ptr<Trie> codebook;
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