#include <fstream>
#include <iostream>

#include "lzw.hpp"

int main() {
  lzw::lzw_encoder encoder(std::cin, std::cout);
  encoder.encode();
}