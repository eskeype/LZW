#include <iostream>
#include <fstream>

#include "lzw.hpp"

int main() {
	lzw::lzw_decoder decoder(std::cin, std::cout);
	decoder.decode();
}