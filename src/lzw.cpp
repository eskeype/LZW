#include "lzw.hpp"

#include <iterator>

namespace lzw {

lzw_encoder::lzw_encoder(std::istream &is, std::ostream &os)
    : is(is), os(os), current_code(0), codebook(std::make_unique<Trie>()) {
  for (; current_code < 256; ++current_code) {
    codebook->children[current_code] = std::make_unique<Trie>(current_code);
  }
}

void lzw_encoder::encode() {
  Trie *current_node = codebook.get();

  for (std::istreambuf_iterator<char> is_iter(is);
       is_iter != std::istreambuf_iterator<char>(); ++is_iter) {
    if (current_node->children[*is_iter]) {
      current_node = current_node->children[*is_iter].get();
    } else {
      current_node->children[*is_iter] = std::make_unique<Trie>(current_code++);

      os.write(reinterpret_cast<char *>(&(current_node->code)),
               sizeof(current_node->code));

      current_node = codebook->children[*is_iter].get();
    }
  }
  if (current_node != codebook.get()) {
    os.write(reinterpret_cast<char *>(&(current_node->code)),
             sizeof(current_node->code));
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