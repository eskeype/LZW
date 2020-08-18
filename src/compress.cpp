#include <iostream>
#include <fstream>

#include "lzw.hpp"

int main() {
	lzw::lzw_encoder encoder(std::cin, std::cout);
	encoder.encode();
}