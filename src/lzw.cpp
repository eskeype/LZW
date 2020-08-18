#include "lzw.hpp"

#include <iterator>

namespace lzw {

lzw_encoder::lzw_encoder(std::istream &is, std::ostream &os)
    : is(is), os(os), current_code(0) {
  for (current_code = 0; current_code < 256; ++current_code) {
    codebook[std::string(1, static_cast<char>(current_code))] = current_code;
  }
}

void lzw_encoder::encode() {
  for (std::istreambuf_iterator<char> is_iter(is);
       is_iter != std::istreambuf_iterator<char>(); ++is_iter) {
    current.push_back(*is_iter);

    auto iter = codebook.find(current);
    if (iter == codebook.end()) {
      codebook[current] = current_code++;

      current.pop_back();
      auto code_val = codebook[current];
      os.write(reinterpret_cast<char *>(&code_val), sizeof(code_val));

      current.clear();
      current.push_back(*is_iter);
    }
  }
  if (current.size()) {
    auto code_val = codebook[current];
    os.write(reinterpret_cast<char *>(&code_val), sizeof(code_val));
  }
}

lzw_decoder::lzw_decoder(std::istream &is, std::ostream &os)
    : is(is), os(os), prev{} {
  for (int i = 0; i < 256; ++i) {
    codebook.emplace_back(1, static_cast<char>(i));
  }
}

void lzw_decoder::decode() {
  uint32_t code;
  while (is.read(reinterpret_cast<char *>(&code), sizeof(uint32_t))) {
    if (code < codebook.size()) {
      os << codebook[code];
      if (prev) {
        codebook.push_back(codebook[*prev] + codebook[code].front());
      }
    } else {
      codebook.push_back(codebook[*prev] + codebook[*prev].front());
      os << codebook.back();
    }
    prev = code;
  }
}
}  // namespace lzw